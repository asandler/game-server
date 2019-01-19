class User < ActiveRecord::Base
  authenticates_with_sorcery!

  validates :password, confirmation: true, length: { in: 6..20 }, allow_blank: true
  validates :password_confirmation, presence: true

  validates :email, uniqueness: true
  validates :email, format: { with: /\A[a-zA-Z0-9]+\z/, message: "Only english letters and digits allowed" }
  validates :email, length: { in: 3..20 }

end
