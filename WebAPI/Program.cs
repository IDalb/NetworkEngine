using System.Text;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;

// Secret key for authentification encryption
string SECRET_KEY = "secretkey01234***secretkey5678***";

string DbPath;
var folder = Environment.SpecialFolder.LocalApplicationData;
var path = Environment.GetFolderPath(folder);
DbPath = Path.Join(path, "cube_game.db");

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContext<GameDb>(opt => opt.UseSqlite($"Data Source={DbPath}"));
builder.Services.AddDatabaseDeveloperPageExceptionFilter();

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddOpenApiDocument(config => {
    config.DocumentName = "GameRestAPI";
    config.Title = "GameRestAPI v1";
    config.Version = "v1.0";
});

builder.Services.AddAuthentication(cfg => {
    cfg.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
    cfg.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
    cfg.DefaultScheme = JwtBearerDefaults.AuthenticationScheme;
}).AddJwtBearer(x => {
    x.RequireHttpsMetadata = false;
    x.SaveToken = false;
    x.TokenValidationParameters = new TokenValidationParameters {
        ValidateIssuerSigningKey = true,
        IssuerSigningKey = new SymmetricSecurityKey(
            Encoding.UTF8.GetBytes(SECRET_KEY)
        ),
        ValidateIssuer = false,
        ValidateAudience = false,
        ValidateLifetime = true,
        ClockSkew = TimeSpan.Zero
    };
});

var app = builder.Build();

if (app.Environment.IsDevelopment()) {
    app.UseOpenApi();
    app.UseSwaggerUi(config => {
        config.DocumentTitle = "GameRestAPI";
        config.Path = "/swagger";
        config.DocumentPath = "/swagger/{documentName}/swagger.json";
        config.DocExpansion = "list";
    });
}

var users = app.MapGroup("/users");
var stats = app.MapGroup("/stats");
var achievements = app.MapGroup("/achievements");
var match = app.MapGroup("/match");

app.MapGet("/", () => "Hello World!");
app.MapPost("/login", RequestLogin);

users.MapGet("/", GetAllUsers);
users.MapGet("/{id}", GetUser);
users.MapPost("/", CreateUser);
users.MapPut("/{id}", UpdateUser);
users.MapDelete("/{id}", DeleteUser);

stats.MapGet("/", GetAllStatistics);
stats.MapGet("/{id}", GetStatistic);
stats.MapGet("/user#{playerId}", GetUserStatistics);
stats.MapPut("/{playerId}", UpdateUserStatistic);

achievements.MapGet("/", GetAllAchievements);
achievements.MapGet("/{id}", GetAchievement);

match.MapGet("/", GetAllMatches);
match.MapGet("/{id}", GetMatch);
match.MapPut("/{id}", UpdateMatch);
match.MapGet("/find", FindMatch);
match.MapPost("/connect", ConnectServer);
match.MapPost("/connect/{id}", ConnectToServer);
match.MapDelete("/disconnect/{id}", DisconnectServer);



// Methods implementation

static async Task<IResult> GetAllUsers(GameDb db) {
    return TypedResults.Ok(await db.Users.Select(x => x).ToListAsync());
}

static async Task<IResult> GetUser(int id, GameDb db) {
    return await db.Users.FindAsync(id) is User user
        ? Results.Ok(new UserDTO(user))
        : Results.NotFound();
}

static async Task<IResult> CreateUser(UserDTO userDTO, GameDb db) {
    var user = new User {
        Username = userDTO.Username,
        Password = userDTO.Password
    };
    
    db.Users.Add(user);
    await db.SaveChangesAsync();
    await InitializeStatistics(user.Id, db);

    userDTO = new UserDTO(user);

    return Results.Created($"/users/{user.Id}", userDTO);
}

static async Task InitializeStatistics(int playerId, GameDb db) {
    if (db.Users.Find(playerId) is not User user)
        return;

    // Add stats to the database
    var statGamesPlayed = new Statistic(playerId, "gamesPlayed", 0f);
    db.Statistics.Add(statGamesPlayed);

    var statGamesWon = new Statistic(playerId, "gamesWon", 0f);
    db.Statistics.Add(statGamesWon);

    var statCubesExpelled = new Statistic(playerId, "cubesExpelled", 0f);
    db.Statistics.Add(statCubesExpelled);
    
    await db.SaveChangesAsync();
    
    // Bind those stats to the user
    user.Statistics.Add(statGamesPlayed.Id);
    user.Statistics.Add(statGamesWon.Id);
    user.Statistics.Add(statCubesExpelled.Id);

    await db.SaveChangesAsync();
}

static async Task<IResult> UpdateUser(int id, UserDTO userDTO, GameDb db) {
    var user = await db.Users.FindAsync(id);
    if (user is null) return Results.NotFound();

    user.Username = userDTO.Username;
    user.Password = userDTO.Password;

    await db.SaveChangesAsync();
    return Results.NoContent();
}

static async Task<IResult> DeleteUser(int id, GameDb db) {
    if (await db.Users.FindAsync(id) is not User user)
        return Results.NotFound();

    // Delete statistics linked to a user
    foreach (Statistic stat in await db.Statistics.Where(x => x.PlayerId == user.Id).ToListAsync())
        db.Statistics.Remove(stat);

    db.Users.Remove(user);
    await db.SaveChangesAsync();
    return Results.NoContent();
}

IResult RequestLogin(UserDTO credentials, GameDb db)
{
    var user = db.Users.Where(x =>
        x.Username == credentials.Username
        && x.Password == credentials.Password
    ).FirstOrDefault();

    if (user is null) return Results.NotFound();

    var token = GenerateJwt(user);
    return Results.Ok(new { token });
}

string GenerateJwt(User user) {
    var claims = new List<Claim> {
        new Claim(ClaimTypes.NameIdentifier, user.Id.ToString()),
        new Claim(ClaimTypes.Name, user.Username)
    };

    var token = new JwtSecurityToken(
        claims: claims,
        notBefore: DateTime.UtcNow,
        expires: DateTime.UtcNow.AddMinutes(1),
        signingCredentials: new SigningCredentials(
            new SymmetricSecurityKey(
                Encoding.UTF8.GetBytes(SECRET_KEY)
            ),
            SecurityAlgorithms.HmacSha256
        )
    );

    return new JwtSecurityTokenHandler().WriteToken(token);
}

static async Task<IResult> GetAllStatistics(GameDb db) {
    return TypedResults.Ok(await db.Statistics.Select(x => x).ToListAsync());
}

static async Task<IResult> GetStatistic(int id, GameDb db) {
    return TypedResults.Ok(await db.Statistics.Where(x => x.Id == id).ToListAsync());
}

static async Task<IResult> GetUserStatistics(int playerId, GameDb db) {
    return TypedResults.Ok(await db.Statistics.Where(x => x.PlayerId == playerId).ToListAsync());
}

static async Task<IResult> UpdateUserStatistic(int playerId, StatisticDTO statDto, GameDb db) {
    var stat = await db.Statistics.Where(x =>
        x.PlayerId == playerId &&
        x.Name == statDto.Name
    ).FirstOrDefaultAsync();
    if (stat is null) return Results.NotFound();

    stat.Value = statDto.Value;

    await db.SaveChangesAsync();
    return Results.NoContent();
}

static async Task<IResult> GetAllAchievements(GameDb db) {
    return TypedResults.Ok(await db.Achievements.Select(x => x).ToListAsync());
}

static async Task<IResult> GetAchievement(int id, GameDb db) {
    return TypedResults.Ok(await db.Achievements.Where(x => x.Id == id).ToListAsync());
}

static async Task<IResult> GetAllMatches(GameDb db) {
    return TypedResults.Ok(await db.Servers.Select(x => x).ToListAsync());
}

static async Task<IResult> GetMatch(int id, GameDb db) {
    return TypedResults.Ok(await db.Servers.Where(x => x.Id == id).ToListAsync());
}

static async Task<IResult> UpdateMatch(int id, MatchServerDTO matchDto, GameDb db) {
    var server = await db.Servers.Where(x => x.Id == id).FirstOrDefaultAsync();
    if (server is null) return Results.NotFound();

    if (matchDto.Ip != string.Empty) server.Ip = matchDto.Ip;
    if (matchDto.Port != default) server.Port = matchDto.Port;
    if (matchDto.CurrentPlayerNumber != default) server.CurrentPlayerNumber = matchDto.CurrentPlayerNumber;
    if (matchDto.ServerState != default) server.ServerState = matchDto.ServerState;

    await db.SaveChangesAsync();
    return Results.NoContent();
}

static async Task<IResult> FindMatch(GameDb db) {
    var servers = await db.Servers.Select(x => x).ToListAsync();
    foreach (var server in servers) {
        if (server.ServerState != ServerState.WAITING) continue;
        if (server.CurrentPlayerNumber >= 4) continue;

        return TypedResults.Ok(server);
    }
    
    return Results.NotFound();
} 

static async Task<IResult> ConnectServer(MatchServerDTO matchDTO, GameDb db) {
    /// TODO
    var server = new MatchServer {
        Ip = matchDTO.Ip,
        Port = matchDTO.Port,
        CurrentPlayerNumber = matchDTO.CurrentPlayerNumber != default ? matchDTO.CurrentPlayerNumber : 0,
        ServerState = matchDTO.ServerState != default ? matchDTO.ServerState : ServerState.WAITING
    };
    
    db.Servers.Add(server);
    await db.SaveChangesAsync();
    
    return Results.Created($"/servers/{server.Id}", server);
}

static async Task<IResult> ConnectToServer(int id, GameDb db) {
    if (await db.Servers.FindAsync(id) is not MatchServer server)
        return Results.NotFound();

    if (server.CurrentPlayerNumber >= 4 || server.ServerState != ServerState.WAITING)
        return Results.Forbid();

    server.CurrentPlayerNumber += 1;
    await db.SaveChangesAsync();

    return Results.Ok();
}

static async Task<IResult> DisconnectServer(int id, GameDb db) {
    if (await db.Servers.FindAsync(id) is not MatchServer server)
        return Results.NotFound();

    // We may want to disconnect players from this server

    db.Servers.Remove(server);
    await db.SaveChangesAsync();
    return Results.NoContent();
}

app.UseAuthentication();
app.Run();
