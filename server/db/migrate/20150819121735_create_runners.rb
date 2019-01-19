class CreateRunners < ActiveRecord::Migration
  def change
    create_table :runners do |t|
      t.timestamps null: false
      t.string :host
      t.string :status
    end
  end
end
