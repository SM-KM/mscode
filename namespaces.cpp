/*
Allow to have methods, declarations with the same name, but that
dont overlap, and can be used both.
*/

namespace Root1 {
int Find(int pos) { return pos + 1; }
} // namespace Root1

namespace Root2 {
int Find(int start, int value, int end) { return (start - end) + value; }
} // namespace Root2

int main(int argc, char *argv[]) {
  Root1::Find(5);
  Root2::Find(0, 5, 10);
  return 0;
}
