<?php
function counter(int $n): Generator {
  for ($i=0; $i<$n; $i++) yield $i * $i;
}
foreach (counter(5) as $v) echo $v."\n";