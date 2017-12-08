MRuby::Gem::Specification.new('mruby-libui') do |spec|
  spec.license = 'MIT'
  spec.authors = 'bamchoh'

  # spec.linker.flags_after_libraries << [File.join(__dir__, "libui.dll")]
  # spec.linker.libraries << ["ui"]
  spec.linker.libraries << ["ui"]
	spec.linker.library_paths << ['D:\src\mruby\mruby-libui']
end
