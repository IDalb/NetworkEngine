using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace GameRestApi.Migrations
{
    /// <inheritdoc />
    public partial class AddPlay10Achievement : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 1,
                columns: new[] { "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { "play10", "gamesPlayed", 10f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 2,
                columns: new[] { "Name", "ThresholdValue" },
                values: new object[] { "win1", 1f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 3,
                columns: new[] { "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { "win5", "gamesWon", 5f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 4,
                columns: new[] { "Aggregation", "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { 0, "cube20", "cubesExpelled", 20f });

            migrationBuilder.InsertData(
                table: "Achievements",
                columns: new[] { "Id", "Aggregation", "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { 5, 1, "time30", "bestTime", 30f });
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DeleteData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 5);

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 1,
                columns: new[] { "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { "win1", "gamesWon", 1f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 2,
                columns: new[] { "Name", "ThresholdValue" },
                values: new object[] { "win5", 5f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 3,
                columns: new[] { "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { "cube20", "cubesExpelled", 20f });

            migrationBuilder.UpdateData(
                table: "Achievements",
                keyColumn: "Id",
                keyValue: 4,
                columns: new[] { "Aggregation", "Name", "StatisticName", "ThresholdValue" },
                values: new object[] { 1, "time30", "bestTime", 30f });
        }
    }
}
