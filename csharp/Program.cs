using System.Text.Json;

record User(int Id, string Name);

class Program
{
    static void Main()
    {
        Console.WriteLine("=== C# demo ===");
        var users = new[] { new User(1, "Ada"), new User(2, "Alan") };
        var csv = string.Join("\n", users.Select(u => $"{u.Id},{u.Name}"));
        Console.WriteLine(csv);

        var json = JsonSerializer.Serialize(users);
        Console.WriteLine(json);

        // Span example
        Console.WriteLine(SpanExample.Sum(new int[]{1,2,3,4}));
    }
}