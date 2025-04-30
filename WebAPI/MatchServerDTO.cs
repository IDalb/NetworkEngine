public class MatchServerDTO
{
    public string Ip { get; set; } = string.Empty;
    public int Port { get; set; }
    public int CurrentPlayerNumber { get; set; }
    public ServerState ServerState;

    public MatchServerDTO(string ip, int port, int currentPlayerNumber, ServerState serverState) => (Ip, Port, CurrentPlayerNumber, ServerState) = (ip, port, currentPlayerNumber, serverState);
    public MatchServerDTO(MatchServer server) => (Ip, Port, CurrentPlayerNumber, ServerState) = (server.Ip, server.Port, server.CurrentPlayerNumber, server.ServerState);
}