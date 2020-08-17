class AddSchedulerFieldsToUsers < ActiveRecord::Migration['4.2']
  def change
    add_column :users, :last_send_timestamp, :timestamp
    add_column :users, :next_send_timestamp, :timestamp
    add_column :users, :sends_today, :integer
    add_column :users, :sends_total, :integer
  end
end
