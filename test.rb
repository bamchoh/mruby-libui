LibUI.init

10.times do |i|
	win = LibUI::Window.new("window on mruby #{i}", 640, 480, 1)

	win.on_closing {
		LibUI.quit
	}

	win.show
end

LibUI.main
