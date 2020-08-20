#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/time.h> 
#include <unistd.h>

#define TYPE_READ 1
#define TYPE_WRITE 2

using namespace std;

class Exception : public exception {
public:
    Exception(const string& _s) throw() : s(_s) {}

    Exception(const string& _s, int t) throw() {
        stringstream str;
        str << _s << t;
        str >> s;
    }

    const string &What() {
        return s;
    }

    virtual const char* what() const throw() {
        return s.c_str();
    }

    virtual ~Exception() throw() {}

private:
    string s;
};

class Logger {
public:
    void Log(const string& s, int turn, int type) {
        string t(1, (char)(turn + '0'));

        t += ((type == TYPE_READ) ? " << " : " >> ") + s;
        if (t[t.length() - 1] == '\n') {
            t.erase(t.length() - 1, 1);
        }

        pool.push_back(t);
    }

    void Log(const string& s, int k, int turn, int type) {
        string t(1, (char)(turn + '0'));

        t += ((type == TYPE_READ) ? " << " : " >> ") + s;
        if (t[t.length() - 1] == '\n') {
            t.erase(t.length() - 1, 1);
        }
        t.push_back((char)(k + '0'));

        pool.push_back(t);
    }

    vector<string>::const_iterator begin() const {
        return pool.begin();
    }

    vector<string>::const_iterator end() const {
        return pool.end();
    }
private:
    vector<string> pool;
};

ostream& operator <<(ostream& os, const Logger& L) {
    for (vector<string>::const_iterator it = L.begin(); it != L.end(); it++) {
        os << *it << std::endl;
    }
    return os;
}

class Humble {
public:
    Humble(const string& p, int _sec, int _usec) : path(p), sec(_sec), usec(_usec) {
        pipe(fd);
        pipe(df);
    }
    void Start() {
        if ((pid = fork()) < 0) {
            throw Exception("Internal error (fork)");
        }

        if (pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);

            dup2(df[0], STDIN_FILENO);
            close(df[1]);

            if (ptrace(static_cast<__ptrace_request>(0x4281), 0, 0, 0) >= 0) {
                cerr << "New interface detected" << endl;
            }

            char* argv[2];
            argv[0] = new char[200];
            argv[1] = NULL;
            strcpy(argv[0], path.c_str());

            if (execvp(path.c_str(), argv) == -1) {
                throw Exception("Internal error (execvp)");
            }

            delete argv[1];
            delete argv;
        }
    }

    void Stop() {
        kill(pid, SIGKILL);
    }

    string ReadFrom() {
        fd_set rfds;
        struct timeval tv;
        FD_ZERO(&rfds);
        FD_SET(fd[0], &rfds);
        tv.tv_sec = sec;
        tv.tv_usec = usec;

        int retval = select(fd[0] + 1, &rfds, NULL, NULL, &tv);
        int i = 0, res = 0;

        char buf[1];
        buf[0] = 'a';
        string s = "";

        if (retval == -1) {
            throw Exception("Internal error (select)");
        } else {
            if (retval) {
                for (i = 0; i <= 256 && buf[0] != '\n'; i++) {
                    if (select(fd[0] + 1, &rfds, NULL, NULL, &tv) <= 0) {
                        throw Exception("Time limit exceeded");
                    }
                    if ((res = read(fd[0], buf, 1)) == -1) {
                        throw Exception("Internal error (read)");
                    }
                    if (res == 0) {
                        s.push_back('\n');
                        break;
                    }
                    s.push_back(buf[0]);
                }
                if (s.length() > 256) {
                    throw Exception("Too long output");
                }
            } else {
                throw Exception("Time limit exceeded");
            }
        }
        return s;
    }

    void WriteTo(const string& s) {
        write(df[1], s.c_str(), s.length());
        if (s[s.length() - 1] != '\n') {
            write(df[1], "\n", 1);
        }
    }

    ~Humble() {
        Stop();
    }

private:
    string path;
    int pid;
    int fd[2], df[2];
    int sec, usec;
};

class Battle {
public:
    Battle(string path1, string path2, string pathc, int sec, int usec)
        : p1(new Humble(path1, sec, usec))
        , p2(new Humble(path2, sec, usec))
        , pc(new Humble(pathc, sec, usec))
        , L(new Logger())
        , win(0)
        {}

    void Run() {
        try {
            p1->Start();
            p2->Start();
            pc->Start();
        } catch (Exception& e) {
            throw e;
        }
    }

    void Init(string path, int turn) {
        if (freopen(path.c_str(), "r", stdin) == NULL) {
            throw Exception("Bad init file");
        }
        string s;
        getline(cin, s);
        int n = atoi(s.c_str());

        //write config to checker
        if (turn == 1) {
            pc->WriteTo("c1");
        } else {
            pc->WriteTo("c2");
        }

        for (int i = 0; i < n; i++) {
            getline(cin, s);
            if (s.length() > 255) {
                throw Exception("Bad init file");
            }

            if (turn == 1) {
                p1->WriteTo(s);
            } else {
                p2->WriteTo(s);
            }
            pc->WriteTo(s);

            L.get()->Log(s, turn, TYPE_READ);
        }

        try {
            s = pc->ReadFrom();
        } catch (Exception& e) {
            throw Exception("Bad checker: " + e.What());
        }

        if (s != "OK\n") {
            cerr << "Checker answer: " << s << endl;
            throw Exception("Bad init file");
        }
    }

    void Fight() {
        int turn = 1;
        string s;
        bool finish = false;

        while (true) {

            // Read move from the game
            try {
                s = (turn == 1) ? p1->ReadFrom() : p2->ReadFrom();
            } catch (Exception& e) {
                //read scores
                try {
                    pc->WriteTo("i");
                    s = pc->ReadFrom();
                } catch (Exception& e) {
                    throw Exception("Bad checker");
                }

                win = 3 - turn;
                L.get()->Log(e.What(), turn, 0, TYPE_WRITE);
                L.get()->Log("Winner: ", win, 0, TYPE_WRITE);

                s.erase(s.begin());
                L.get()->Log("Score: " + s, 0, TYPE_WRITE);

                throw Exception(e.What(), turn);
            }

            // Logging the move
            L.get()->Log(s, turn, TYPE_WRITE);

            // Checking the move
            try {
                pc->WriteTo(" " + s);
                s = pc->ReadFrom();
            } catch (Exception& e) {
                throw Exception("Bad checker");
            }

            // Parsing the answer of checker
            switch (s[0]) {
                case 'i':
                    win = 3 - turn;
                    s.erase(s.begin());
                    L.get()->Log(s, 0, TYPE_WRITE);
                    L.get()->Log("Winner: ", win, 0, TYPE_WRITE);

                    try {
                        pc->WriteTo("f");
                        s = pc->ReadFrom();
                        s.erase(s.begin());
                        L.get()->Log("Score: " + s, 0, TYPE_WRITE);
                    } catch (Exception& e) {
                        throw Exception("Bad checker");
                    }

                    throw Exception("Presentation error");
                    break;

                case '0':
                case '1':
                case '2':
                    win = s[0] - '0';
                    L.get()->Log("Winner: ", win, 0, TYPE_WRITE);
                    s.erase(s.begin());
                    L.get()->Log("Score: " + s, 0, TYPE_WRITE);
                    finish = true;
                    break;

                default:
                    s.erase(s.begin());
                    break;
            }

            if (finish) {
                break;
            }

            // Pass move to another player
            turn = 3 - turn;
            if (turn == 1) {
                p1->WriteTo(s);
            } else {
                p2->WriteTo(s);
            }

            //Logging the move
            L.get()->Log(s, turn, TYPE_READ);
        }
    }

    const Logger& Log() const {
        return *L;
    }

    int Win() const {
        return win;
    }

private:
    auto_ptr<Humble> p1, p2, pc;
    auto_ptr<Logger> L;
    int win;
};

int main(int argc, char** argv) {
    if (argc < 8) {
        cerr << "battle: too few parametres" << endl;
        return -1;
    }

    string game1, game2, checker, init1, init2;
    game1.assign(argv[1]);
    game2.assign(argv[2]);
    checker.assign(argv[3]);
    init1.assign(argv[4]);
    init2.assign(argv[5]);
    int sec = atoi(argv[6]);
    int usec = atoi(argv[7]);

    Battle B(game1, game2, checker, sec, usec);

    try {
        B.Run();
        B.Init(init1, 1);
        B.Init(init2, 2);
        B.Fight();
    } catch (exception& e) {
        cerr << e.what() << endl;
        cout << B.Log();
        return -1;
    }

    cout << B.Log();
    return B.Win();
}
