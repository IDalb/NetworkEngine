public class User
{
    public int Id { get; set; }
    public string Username { get; set; } = string.Empty;
    public string Password { get; set; } = string.Empty;

    public List<int> Statistics { get; set; } = [];
    public List<int> Achievements { get; set; } = [];


    // Returns the list of changes
    public int[] UpdateAchievements(GameDb db) {
        var playerAchievements = new List<int>();
        
        var achievements = db.Achievements.Select(x => x).ToList();
        
        foreach(Achievement achievement in achievements) {
            if (achievement.CheckAchievement(Id, db))
                playerAchievements.Add(achievement.Id);
        }
        
        Achievements = playerAchievements;
        db.SaveChanges();
        
        return playerAchievements.ToArray();
    }
}