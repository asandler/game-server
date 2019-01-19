# 0) put custom .vimrc and .gemrc to a machine

# 1) some packages
sudo apt-get update
sudo apt-get install fpc git htop kernel-package libmagickwand-dev imagemagick
sudo locale-gen ru_RU ru_RU.UTF-8
sudo dpkg-reconfigure locales

# 2) rvm and ruby
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
\curl -sSL https://get.rvm.io | bash

# LOGOUT

rvm install 2.3.0
sudo ln -s /home/ubuntu/.rvm/rubies/ruby-2.3.0/bin/ruby /bin/ruby

# LOGOUT

# 3) gems

gem install fileutils
gem install sinatra
gem install thin

# 4) repo

git clone https://bitbucket.org/asandler/game-runner

# 5) kernel

wget https://www.kernel.org/pub/linux/kernel/v3.0/linux-3.12.50.tar.gz
gunzip linux-3.12.50.tar.gz
tar xf linux-3.12.50.tar && rm linux-3.12.50.tar
cd linux-3.12.50/
wget https://www.ejudge.ru/download/linux-3.12.50-3.12.50-cher1.diff --no-check-certificate
patch -p1 < linux-3.12.50-3.12.50-cher1.diff 
cp /boot/config-3.13.0-74-generic ./.config
make menuconfig
make-kpkg --initrd kernel_image kernel_headers
