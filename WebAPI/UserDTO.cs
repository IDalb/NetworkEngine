public class UserDTO {
    public string Username { get; set; } = string.Empty;
    public string Password { get; set; } = string.Empty;

    public UserDTO() {}
    public UserDTO(User userItem) =>
    (Username, Password) = (userItem.Username, userItem.Password);
}