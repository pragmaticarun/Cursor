<?php
use PHPUnit\Framework\TestCase;

final class DemoTest extends TestCase {
  public function testBasic(): void {
    $this->assertSame(4, 2+2);
  }
}