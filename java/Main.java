import java.nio.file.*;
import java.time.*;
import java.util.*;
import java.util.stream.*;

public class Main {
    public static void main(String[] args) throws Exception {
        System.out.println("=== Java demo ===");

        record User(int id, String name) {}
        List<User> users = List.of(new User(1, "Ada"), new User(2, "Alan"));
        String csv = users.stream().map(u -> u.id()+","+u.name()).collect(Collectors.joining("\n"));
        System.out.println(csv);

        Path here = Path.of(".").toAbsolutePath().normalize();
        try (Stream<Path> s = Files.list(here)) {
            System.out.println("Some entries:");
            s.limit(3).forEach(p -> System.out.println("- " + p));
        }

        System.out.println("Now: " + Instant.now());
    }
}