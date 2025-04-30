public class User
{
    public int Id { get; set; }
    public string Username { get; set; } = string.Empty;
    public string Password { get; set; } = string.Empty;

    public List<int> Statistics { get; set; } = [];
    public List<int> Achievements { get; set; } = [];


    // Returns the list of changes
    public AchievementChange[] UpdateAchievements(GameDb db) {
        var changes = new List<AchievementChange>();
        
        var achievements = db.Achievements.Select(x => x).ToList();
        
        foreach(Achievement achievement in achievements) {
            if (achievement.CheckAchievement(Id, db)) {
                if (Achievements.Contains(achievement.Id)) continue;
                
                // Grant a new achievement
                Achievements.Add(achievement.Id);
                changes.Add(new AchievementChange(achievement.Id, true));
            }
            else {
                if (!Achievements.Contains(achievement.Id)) continue;

                // Revoke an achievement
                Achievements.Remove(achievement.Id);
                changes.Add(new AchievementChange(achievement.Id, false));
            }
        }
        
        return changes.ToArray();
    }
}