public enum AchievementAggregation {
    GREATER_THAN,   // Inclusive
    LESS_THAN       // Inclusive
}

public class Achievement
{
    public int Id { get; set; }
    public string Name { get; set; } = string.Empty;
    public string StatisticName { get; set; } = string.Empty;
    public AchievementAggregation Aggregation { get; set; }
    public float ThresholdValue { get; set; }

    public Achievement() {}

    public bool CheckAchievement(int playerId, GameDb db) {
        if (db.Statistics.Where(x => x.PlayerId == playerId && x.Name == StatisticName).FirstOrDefault() is not Statistic stat)
            return false;   // Stat not found fot this player

        return Aggregation switch
        {
            AchievementAggregation.GREATER_THAN => stat.Value >= ThresholdValue,
            AchievementAggregation.LESS_THAN => stat.Value <= ThresholdValue,
            _ => false
        };
    }
}