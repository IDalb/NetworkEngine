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
    public int CurrentPlayerNumber { get; set; }
    public ServerState ServerState;

    public MatchServer() {}
    public MatchServer(string ip, int port) => (Ip, Port, CurrentPlayerNumber, ServerState) = (ip, port, 0, ServerState.WAITING);
}