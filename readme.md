# WS_CAPTION-less window problems

This sample project illustrates WinAPI problems related to `WS_CAPTION` absence.

* Regardless the fact we don't set WS_CAPTION in dwStyle, the window was created WITH CAPTION
* In Win8.1 the caption without text is still visible: 2018-01-29_13-26-20.png
* After dragging the window on this caption we see fake non-themed caption: 2018-01-29_13-26-48.png
