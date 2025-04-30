public class Statistic
{
    public int Id { get; set; }
    public int PlayerId { get; set; }                   // Not the Id of the statistic, but the Id of the player it is attached to
    public string Name { get; set; } = string.Empty;    // Name used as an identification for the statistic
    public float Value { get; set; }

    public Statistic(int playerId, string name, float value) => (PlayerId, Name, Value) = (playerId, name, value);
}