public class MatchServerDTO
{
    public int Id { get; set; }
    public string Ip { get; set; } = string.Empty;
    public int Port { get; set; }
    public List<int> CurrentPlayers { get; set; } = [];
    public int ServerState;

    public MatchServerDTO() {}
    public MatchServerDTO(MatchServer server) => (Id, Ip, Port, CurrentPlayers, ServerState) = (server.Id, server.Ip, server.Port, server.CurrentPlayers, (int) server.ServerState);
}