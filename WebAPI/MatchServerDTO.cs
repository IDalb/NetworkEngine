public class MatchServerDTO
{
    public string Ip { get; set; } = string.Empty;
    public int Port { get; set; }
    public List<int> CurrentPlayers { get; set; } = [];
    public int ServerState;

    public MatchServerDTO() {}
    public MatchServerDTO(MatchServer server) => (Ip, Port, CurrentPlayers, ServerState) = (server.Ip, server.Port, server.CurrentPlayers, (int) server.ServerState);
}