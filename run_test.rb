#!/usr/bin/env ruby

if __FILE__ == $PROGRAM_NAME
  require 'fileutils'
  FileUtils.mkdir_p 'tmp'
  unless File.exists?('tmp/mruby')
    system 'git clone https://github.com/mruby/mruby.git tmp/mruby'
  end
  system(%Q[cd tmp/mruby && ruby minirake #{ARGV.join(' ')} MRUBY_CONFIG=\"#{File.expand_path __FILE__}\"])
	exit system(%Q[tmp/mruby/bin/mruby test.rb])
end

MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'

  conf.gem File.dirname(File.expand_path(__FILE__))

  conf.cc.command = "x86_64-w64-mingw32-gcc"
  conf.linker.command = "x86_64-w64-mingw32-gcc"
end

