##
## LibUI Test
##

assert("LibUI#hello") do
  t = LibUI.new "hello"
  assert_equal("hello", t.hello)
end

assert("LibUI#bye") do
  t = LibUI.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("LibUI.hi") do
  assert_equal("hi!!", LibUI.hi)
end
