using System.Diagnostics;
using Microsoft.EntityFrameworkCore;

public class GameDb : DbContext
{
    public DbSet<User> Users => Set<User>();
    public DbSet<Statistic> Statistics => Set<Statistic>();
    public DbSet<Achievement> Achievements => Set<Achievement>();
    public DbSet<MatchServer> Servers => Set<MatchServer>();
    
    private bool AchievementsExists(int id) {
        foreach(Achievement achievement in Achievements)
            if (achievement.Id == id) return true;
        return false;
    }

    public GameDb(DbContextOptions<GameDb> options): base(options) {}

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        // Hard-coded achievements
        var win1 = new Achievement {
            Id = 1,
            Name = "win1",
            StatisticName = "gamesWon",
            Aggregation = AchievementAggregation.GREATER_THAN,
            ThresholdValue = 1f
        };
        var win5 = new Achievement {
            Id = 2,
            Name = "win5",
            StatisticName = "gamesWon",
            Aggregation = AchievementAggregation.GREATER_THAN,
            ThresholdValue = 5f
        };
        var cube20 = new Achievement {
            Id = 3,
            Name = "cube20",
            StatisticName = "cubesExpelled",
            Aggregation = AchievementAggregation.GREATER_THAN,
            ThresholdValue = 20f
        };
        var time30 = new Achievement {
            Id = 4,
            Name = "time30",
            StatisticName = "bestTime",
            Aggregation = AchievementAggregation.LESS_THAN,
            ThresholdValue = 30f
        };

        modelBuilder.Entity<Achievement>().HasData(win1, win5, cube20, time30);

        base.OnModelCreating(modelBuilder);
    }

}