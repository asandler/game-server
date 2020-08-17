class CreateRunners < ActiveRecord::Migration['4.2']
  def change
    create_table :runners do |t|
      t.timestamps null: false
      t.string :host
      t.string :status
    end
  end
end
