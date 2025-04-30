public class StatisticDTO
{
    public string Name { get; set; } = string.Empty;
    public float Value { get; set; }

    public StatisticDTO() {}
    public StatisticDTO(Statistic stat) => (Name, Value) = (stat.Name, stat.Value);
}