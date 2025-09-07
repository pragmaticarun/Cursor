<?php

declare(strict_types=1);

echo "=== PHP demo ===\n";

class User implements JsonSerializable {
    public function __construct(public int $id, public string $name) {}
    public function jsonSerialize(): mixed { return [ 'id' => $this->id, 'name' => $this->name ]; }
}

$users = [ new User(1, 'Ada'), new User(2, 'Alan') ];
echo json_encode($users, JSON_PRETTY_PRINT) . "\n";

$text = 'Hello hello world';
$words = preg_split('/\W+/', strtolower($text), flags: PREG_SPLIT_NO_EMPTY);
$counts = array_count_values($words);
print_r($counts);

$here = getcwd();
$entries = array_slice(scandir($here), 0, 5);
foreach ($entries as $e) { echo "- $e\n"; }