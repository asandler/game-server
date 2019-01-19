class SorceryCore < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :username
      t.string :email, :null => false
      t.string :crypted_password
      t.string :salt

      t.timestamps

      t.integer :tab_number
    end

    add_index :users, :email, unique: true
  end
end
