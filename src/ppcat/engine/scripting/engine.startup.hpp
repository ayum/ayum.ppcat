namespace ppcat::engine::scripting {
static inline constexpr const char *startup_script = R"(
for (var i = 1; i < 10; ++i)
{
  print(i); // prints 1 through 9
}
)";
}
