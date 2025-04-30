public enum ServerState {
    WAITING,
    INGAME,
    ENDED
}

public class MatchServer
{
    public int Id { get; set; }
    public string Ip { get; set; } = string.Empty;
    public int Port { get; set; }
    public List<int> CurrentPlayers { get; set; } = [];
    public ServerState ServerState;
}