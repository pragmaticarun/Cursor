using System;

public static class SpanExample
{
    public static int Sum(ReadOnlySpan<int> data)
    {
        int s = 0; foreach (var x in data) s += x; return s;
    }
}