// Pure policy shared by runtime and automated regression tests.
class DZES_Issue15Rules
{
 static bool IdleExpired(int now, int since, int seconds, int participants, bool waiting)
 {
  return waiting && participants == 0 && since >= 0 && now >= since && now - since >= seconds * 1000;
 }
}
