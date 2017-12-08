# mruby-libui   [![Build Status](https://travis-ci.org/bamchoh/mruby-libui.svg?branch=master)](https://travis-ci.org/bamchoh/mruby-libui)
LibUI class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'bamchoh/mruby-libui'
end
```
## example
```ruby
p LibUI.hi
#=> "hi!!"
t = LibUI.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
