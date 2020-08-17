class CreateTickets < ActiveRecord::Migration['4.2']
  def change
    create_table :tickets do |t|
      t.string :text
      t.string :status
      t.string :author

      t.timestamps null: false
    end
  end
end
