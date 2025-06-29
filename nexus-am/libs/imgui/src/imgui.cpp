// dear imgui, v1.50 WIP
// (main code and documentation)

// See ImGui::ShowTestWindow() in imgui_demo.cpp for demo code.
// Newcomers, read 'Programmer guide' below for notes on how to setup ImGui in
// your codebase.
// Get latest version at https://github.com/ocornut/imgui
// Releases change-log at https://github.com/ocornut/imgui/releases
// Gallery (please post your screenshots/video there!):
// https://github.com/ocornut/imgui/issues/772
// Developed by Omar Cornut and every direct or indirect contributors to the
// GitHub.
// This library is free but I need your support to sustain development and
// maintenance.
// If you work for a company, please consider financial support, e.g:
// https://www.patreon.com/imgui

/*

 Index
 - MISSION STATEMENT
 - END-USER GUIDE
 - PROGRAMMER GUIDE (read me!)
 - API BREAKING CHANGES (read me when you update!)
 - FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
   - How can I help?
   - How do I update to a newer version of ImGui?
   - What is ImTextureID and how do I display an image?
   - I integrated ImGui in my engine and the text or lines are blurry..
   - I integrated ImGui in my engine and some elements are clipping or
disappearing when I move windows around..
   - How can I have multiple widgets with the same label? Can I have widget
without a label? (Yes). A primer on the purpose of labels/IDs.
   - How can I tell when ImGui wants my mouse/keyboard inputs and when I can
pass them to my application?
   - How can I load a different font than the default?
   - How can I easily use icons in my application?
   - How can I load multiple fonts?
   - How can I display and input non-latin characters such as Chinese, Japanese,
Korean, Cyrillic?
   - How can I use the drawing facilities without an ImGui window? (using
ImDrawList API)
 - ISSUES & TODO-LIST
 - CODE


 MISSION STATEMENT
 =================

 - easy to use to create code-driven and data-driven tools
 - easy to use to create ad hoc short-lived tools and long-lived, more elaborate
tools
 - easy to hack and improve
 - minimize screen real-estate usage
 - minimize setup and maintenance
 - minimize state storage on user side
 - portable, minimize dependencies, run on target (consoles, phones, etc.)
 - efficient runtime (NB- we do allocate when "growing" content - creating a
window / opening a tree node for the first time, etc. - but a typical frame
won't allocate anything)
 - read about immediate-mode gui principles @ http://mollyrocket.com/861,
http://mollyrocket.com/forums/index.html

 Designed for developers and content-creators, not the typical end-user! Some of
the weaknesses includes:
 - doesn't look fancy, doesn't animate
 - limited layout features, intricate layouts are typically crafted in code
 - occasionally uses statically sized buffers for string manipulations - won't
crash, but some very long pieces of text may be clipped. functions like
ImGui::TextUnformatted() don't have such restriction.


 END-USER GUIDE
 ==============

 - double-click title bar to collapse window
 - click upper right corner to close a window, available when 'bool* p_open' is
passed to ImGui::Begin()
 - click and drag on lower right corner to resize window
 - click and drag on any empty space to move window
 - double-click/double-tap on lower right corner grip to auto-fit to content
 - TAB/SHIFT+TAB to cycle through keyboard editable fields
 - use mouse wheel to scroll
 - use CTRL+mouse wheel to zoom window contents (if IO.FontAllowScaling is true)
 - CTRL+Click on a slider or drag box to input value as text
 - text editor:
   - Hold SHIFT or use mouse to select text.
   - CTRL+Left/Right to word jump
   - CTRL+Shift+Left/Right to select words
   - CTRL+A our Double-Click to select all
   - CTRL+X,CTRL+C,CTRL+V to use OS clipboard
   - CTRL+Z,CTRL+Y to undo/redo
   - ESCAPE to revert text to its original value
   - You can apply arithmetic operators +,*,/ on numerical values. Use +- to
subtract (because - would set a negative value!)


 PROGRAMMER GUIDE
 ================

 - read the FAQ below this section!
 - your code creates the UI, if your code doesn't run the UI is gone! == very
dynamic UI, no construction/destructions steps, less data retention on your
side, no state duplication, less sync, less bugs.
 - call and read ImGui::ShowTestWindow() for demo code demonstrating most
features.
 - see examples/ folder for standalone sample applications. Prefer reading
examples/opengl2_example/ first as it is the simplest.
   you may be able to grab and copy a ready made imgui_impl_*** file from the
examples/.
 - customization: PushStyleColor()/PushStyleVar() or the style editor to tweak
the look of the interface (e.g. if you want a more compact UI or a different
color scheme).

 - getting started:
   - init: call ImGui::GetIO() to retrieve the ImGuiIO structure and fill the
fields marked 'Settings'.
   - init: call io.Fonts->GetTexDataAsRGBA32(...) and load the font texture
pixels into graphics memory.
   - every frame:
      1/ in your mainloop or right after you got your keyboard/mouse info, call
ImGui::GetIO() and fill the fields marked 'Input'
      2/ call ImGui::NewFrame() as early as you can!
      3/ use any ImGui function you want between NewFrame() and Render()
      4/ call ImGui::Render() as late as you can to end the frame and finalize
render data. it will call your RenderDrawListFn handler that you set in the IO
structure.
         (if you don't need to render, you still need to call Render() and
ignore the callback, or call EndFrame() instead. if you call neither some
aspects of windows focusing/moving will appear broken.)
   - all rendering information are stored into command-lists until
ImGui::Render() is called.
   - ImGui never touches or know about your GPU state. the only function that
knows about GPU is the RenderDrawListFn handler that you provide.
   - effectively it means you can create widgets at any time in your code,
regardless of considerations of being in "update" vs "render" phases of your own
application.
   - refer to the examples applications in the examples/ folder for instruction
on how to setup your code.
   - a typical application skeleton may be:

        // Application init
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = 1920.0f;
        io.DisplaySize.y = 1280.0f;
        io.IniFilename = "imgui.ini";
        io.RenderDrawListsFn = my_render_function;  // Setup a render function,
or set to NULL and call GetDrawData() after Render() to access the render data.
        // TODO: Fill others settings of the io structure

        // Load texture atlas
        // There is a default font so you don't need to care about choosing a
font yet
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(pixels, &width, &height);
        // TODO: At this points you've got a texture pointed to by 'pixels' and
you need to upload that your your graphic system
        // TODO: Store your texture pointer/identifier (whatever your engine
uses) in 'io.Fonts->TexID'

        // Application main loop
        while (true)
        {
            // 1) get low-level inputs (e.g. on Win32, GetKeyboardState(), or
poll your events, etc.)
            // TODO: fill all fields of IO structure and call NewFrame
            ImGuiIO& io = ImGui::GetIO();
            io.DeltaTime = 1.0f/60.0f;
            io.MousePos = mouse_pos;
            io.MouseDown[0] = mouse_button_0;
            io.MouseDown[1] = mouse_button_1;
            io.KeysDown[i] = ...

            // 2) call NewFrame(), after this point you can use ImGui::*
functions anytime
            ImGui::NewFrame();

            // 3) most of your application code here
            MyGameUpdate(); // may use any ImGui functions, e.g.
ImGui::Begin("My window"); ImGui::Text("Hello, world!"); ImGui::End();
            MyGameRender(); // may use any ImGui functions

            // 4) render & swap video buffers
            ImGui::Render();
            SwapBuffers();
        }

   - You can read back 'io.WantCaptureMouse', 'io.WantCaptureKeybord' etc. flags
from the IO structure to tell how ImGui intends to use your
     inputs and to know if you should share them or hide them from the rest of
your application. Read the FAQ below for more information.


 API BREAKING CHANGES
 ====================

 Occasionally introducing changes that are breaking the API. The breakage are
generally minor and easy to fix.
 Here is a change-log of API breaking changes, if you are using one of the
functions listed, expect to have to fix some code.
 Also read releases logs https://github.com/ocornut/imgui/releases for more
details.

 - 2017/05/26 (1.50) - Removed ImFontConfig::MergeGlyphCenterV in favor of a
more multipurpose ImFontConfig::GlyphOffset.
 - 2017/05/01 (1.50) - Renamed ImDrawList::PathFill() (rarely used directly) to
ImDrawList::PathFillConvex() for clarity.
 - 2016/11/06 (1.50) - BeginChild(const char*) now applies the stack id to the
provided label, consistently with other functions as it should always have been.
It shouldn't affect you unless (extremely unlikely) you were appending multiple
times to a same child from different locations of the stack id. If that's the
case, generate an id with GetId() and use it instead of passing string to
BeginChild().
 - 2016/10/15 (1.50) - avoid 'void* user_data' parameter to
io.SetClipboardTextFn/io.GetClipboardTextFn pointers. We pass
io.ClipboardUserData to it.
 - 2016/09/25 (1.50) - style.WindowTitleAlign is now a ImVec2 (ImGuiAlign enum
was removed). set to (0.5f,0.5f) for horizontal+vertical centering, (0.0f,0.0f)
for upper-left, etc.
 - 2016/07/30 (1.50) - SameLine(x) with x>0.0f is now relative to left of
column/group if any, and not always to left of window. This was sort of always
the intent and hopefully breakage should be minimal.
 - 2016/05/12 (1.49) - title bar (using ImGuiCol_TitleBg/ImGuiCol_TitleBgActive
colors) isn't rendered over a window background (ImGuiCol_WindowBg color)
anymore.
                       If your TitleBg/TitleBgActive alpha was 1.0f or you are
using the default theme it will not affect you.
                       However if your TitleBg/TitleBgActive alpha was <1.0f you
need to tweak your custom theme to readjust for the fact that we don't draw a
WindowBg background behind the title bar.
                       This helper function will convert an old
TitleBg/TitleBgActive color into a new one with the same visual output, given
the OLD color and the OLD WindowBg color.
                           ImVec4 ConvertTitleBgCol(const ImVec4& win_bg_col,
const ImVec4& title_bg_col)
                           {
                               float new_a = 1.0f - ((1.0f - win_bg_col.w) *
(1.0f - title_bg_col.w)), k = title_bg_col.w / new_a;
                               return ImVec4((win_bg_col.x * win_bg_col.w +
title_bg_col.x) * k, (win_bg_col.y * win_bg_col.w + title_bg_col.y) * k,
(win_bg_col.z * win_bg_col.w + title_bg_col.z) * k, new_a);
                           }
                       If this is confusing, pick the RGB value from title bar
from an old screenshot and apply this as TitleBg/TitleBgActive. Or you may just
create TitleBgActive from a tweaked TitleBg color.
 - 2016/05/07 (1.49) - removed confusing set of GetInternalState(),
GetInternalStateSize(), SetInternalState() functions. Now using CreateContext(),
DestroyContext(), GetCurrentContext(), SetCurrentContext().
 - 2016/05/02 (1.49) - renamed SetNextTreeNodeOpened() to SetNextTreeNodeOpen(),
no redirection.
 - 2016/05/01 (1.49) - obsoleted old signature of CollapsingHeader(const char*
label, const char* str_id = NULL, bool display_frame = true, bool default_open =
false) as extra parameters were badly designed and rarely used. You can replace
the "default_open = true" flag in new API with CollapsingHeader(label,
ImGuiTreeNodeFlags_DefaultOpen).
 - 2016/04/26 (1.49) - changed ImDrawList::PushClipRect(ImVec4 rect) to
ImDraw::PushClipRect(Imvec2 min,ImVec2 max,bool
intersect_with_current_clip_rect=false). Note that higher-level
ImGui::PushClipRect() is preferable because it will clip at logic/widget level,
whereas ImDrawList::PushClipRect() only affect your renderer.
 - 2016/04/03 (1.48) - removed style.WindowFillAlphaDefault setting which was
redundant. Bake default BG alpha inside style.Colors[ImGuiCol_WindowBg] and all
other Bg color values. (ref github issue #337).
 - 2016/04/03 (1.48) - renamed ImGuiCol_TooltipBg to ImGuiCol_PopupBg, used by
popups/menus and tooltips. popups/menus were previously using ImGuiCol_WindowBg.
(ref github issue #337)
 - 2016/03/21 (1.48) - renamed GetWindowFont() to GetFont(), GetWindowFontSize()
to GetFontSize(). Kept inline redirection function (will obsolete).
 - 2016/03/02 (1.48) - InputText() completion/history/always callbacks: if you
modify the text buffer manually (without using DeleteChars()/InsertChars()
helper) you need to maintain the BufTextLen field. added an assert.
 - 2016/01/23 (1.48) - fixed not honoring exact width passed to PushItemWidth(),
previously it would add extra FramePadding.x*2 over that width. if you had
manual pixel-perfect alignment in place it might affect you.
 - 2015/12/27 (1.48) - fixed ImDrawList::AddRect() which used to render a
rectangle 1 px too large on each axis.
 - 2015/12/04 (1.47) - renamed Color() helpers to ValueColor() - dangerously
named, rarely used and probably to be made obsolete.
 - 2015/08/29 (1.45) - with the addition of horizontal scrollbar we made various
fixes to inconsistencies with dealing with cursor position.
                       GetCursorPos()/SetCursorPos() functions now include the
scrolled amount. It shouldn't affect the majority of users, but take note that
SetCursorPosX(100.0f) puts you at +100 from the starting x position which may
include scrolling, not at +100 from the window left side.
                       GetContentRegionMax()/GetWindowContentRegionMin()/GetWindowContentRegionMax()
functions allow include the scrolled amount. Typically those were used in cases
where no scrolling would happen so it may not be a problem, but watch out!
 - 2015/08/29 (1.45) - renamed style.ScrollbarWidth to style.ScrollbarSize
 - 2015/08/05 (1.44) - split imgui.cpp into extra files: imgui_demo.cpp
imgui_draw.cpp imgui_internal.h that you need to add to your project.
 - 2015/07/18 (1.44) - fixed angles in ImDrawList::PathArcTo(), PathArcToFast()
(introduced in 1.43) being off by an extra PI for no justifiable reason
 - 2015/07/14 (1.43) - add new ImFontAtlas::AddFont() API. For the old
AddFont***, moved the 'font_no' parameter of ImFontAtlas::AddFont** functions to
the ImFontConfig structure.
                       you need to render your textured triangles with bilinear
filtering to benefit from sub-pixel positioning of text.
 - 2015/07/08 (1.43) - switched rendering data to use indexed rendering. this is
saving a fair amount of CPU/GPU and enables us to get anti-aliasing for a
marginal cost.
                       this necessary change will break your rendering function!
the fix should be very easy. sorry for that :(
                     - if you are using a vanilla copy of one of the
imgui_impl_XXXX.cpp provided in the example, you just need to update your copy
and you can ignore the rest.
                     - the signature of the io.RenderDrawListsFn handler has
changed!
                            ImGui_XXXX_RenderDrawLists(ImDrawList** const
cmd_lists, int cmd_lists_count)
                       became:
                            ImGui_XXXX_RenderDrawLists(ImDrawData* draw_data).
                              argument   'cmd_lists'        ->
'draw_data->CmdLists'
                              argument   'cmd_lists_count'  ->
'draw_data->CmdListsCount'
                              ImDrawList 'commands'         -> 'CmdBuffer'
                              ImDrawList 'vtx_buffer'       -> 'VtxBuffer'
                              ImDrawList  n/a               -> 'IdxBuffer' (new)
                              ImDrawCmd  'vtx_count'        -> 'ElemCount'
                              ImDrawCmd  'clip_rect'        -> 'ClipRect'
                              ImDrawCmd  'user_callback'    -> 'UserCallback'
                              ImDrawCmd  'texture_id'       -> 'TextureId'
                     - each ImDrawList now contains both a vertex buffer and an
index buffer. For each command, render ElemCount/3 triangles using indices from
the index buffer.
                     - if you REALLY cannot render indexed primitives, you can
call the draw_data->DeIndexAllBuffers() method to de-index the buffers. This is
slow and a waste of CPU/GPU. Prefer using indexed rendering!
                     - refer to code in the examples/ folder or ask on the
GitHub if you are unsure of how to upgrade. please upgrade!
 - 2015/07/10 (1.43) - changed SameLine() parameters from int to float.
 - 2015/07/02 (1.42) - renamed SetScrollPosHere() to SetScrollFromCursorPos().
Kept inline redirection function (will obsolete).
 - 2015/07/02 (1.42) - renamed GetScrollPosY() to GetScrollY(). Necessary to
reduce confusion along with other scrolling functions, because positions (e.g.
cursor position) are not equivalent to scrolling amount.
 - 2015/06/14 (1.41) - changed ImageButton() default bg_col parameter from
(0,0,0,1) (black) to (0,0,0,0) (transparent) - makes a difference when texture
have transparence
 - 2015/06/14 (1.41) - changed Selectable() API from (label, selected, size) to
(label, selected, flags, size). Size override should have been rarely be used.
Sorry!
 - 2015/05/31 (1.40) - renamed GetWindowCollapsed() to IsWindowCollapsed() for
consistency. Kept inline redirection function (will obsolete).
 - 2015/05/31 (1.40) - renamed IsRectClipped() to IsRectVisible() for
consistency. Note that return value is opposite! Kept inline redirection
function (will obsolete).
 - 2015/05/27 (1.40) - removed the third 'repeat_if_held' parameter from
Button() - sorry! it was rarely used and inconsistent. Use
PushButtonRepeat(true) / PopButtonRepeat() to enable repeat on desired buttons.
 - 2015/05/11 (1.40) - changed BeginPopup() API, takes a string identifier
instead of a bool. ImGui needs to manage the open/closed state of popups. Call
OpenPopup() to actually set the "open" state of a popup. BeginPopup() returns
true if the popup is opened.
 - 2015/05/03 (1.40) - removed style.AutoFitPadding, using style.WindowPadding
makes more sense (the default values were already the same).
 - 2015/04/13 (1.38) - renamed IsClipped() to IsRectClipped(). Kept inline
redirection function until 1.50.
 - 2015/04/09 (1.38) - renamed ImDrawList::AddArc() to ImDrawList::AddArcFast()
for compatibility with future API
 - 2015/04/03 (1.38) - removed ImGuiCol_CheckHovered, ImGuiCol_CheckActive,
replaced with the more general ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive.
 - 2014/04/03 (1.38) - removed support for passing -FLT_MAX..+FLT_MAX as the
range for a SliderFloat(). Use DragFloat() or Inputfloat() instead.
 - 2015/03/17 (1.36) - renamed
GetItemBoxMin()/GetItemBoxMax()/IsMouseHoveringBox() to
GetItemRectMin()/GetItemRectMax()/IsMouseHoveringRect(). Kept inline redirection
function until 1.50.
 - 2015/03/15 (1.36) - renamed style.TreeNodeSpacing to style.IndentSpacing,
ImGuiStyleVar_TreeNodeSpacing to ImGuiStyleVar_IndentSpacing
 - 2015/03/13 (1.36) - renamed GetWindowIsFocused() to IsWindowFocused(). Kept
inline redirection function until 1.50.
 - 2015/03/08 (1.35) - renamed style.ScrollBarWidth to style.ScrollbarWidth
(casing)
 - 2015/02/27 (1.34) - renamed OpenNextNode(bool) to SetNextTreeNodeOpened(bool,
ImGuiSetCond). Kept inline redirection function until 1.50.
 - 2015/02/27 (1.34) - renamed ImGuiSetCondition_*** to ImGuiSetCond_***, and
_FirstUseThisSession becomes _Once.
 - 2015/02/11 (1.32) - changed text input callback ImGuiTextEditCallback return
type from void-->int. reserved for future use, return 0 for now.
 - 2015/02/10 (1.32) - renamed GetItemWidth() to CalcItemWidth() to clarify its
evolving behavior
 - 2015/02/08 (1.31) - renamed GetTextLineSpacing() to
GetTextLineHeightWithSpacing()
 - 2015/02/01 (1.31) - removed IO.MemReallocFn (unused)
 - 2015/01/19 (1.30) - renamed ImGuiStorage::GetIntPtr()/GetFloatPtr() to
GetIntRef()/GetIntRef() because Ptr was conflicting with actual pointer storage
functions.
 - 2015/01/11 (1.30) - big font/image API change! now loads TTF file. allow for
multiple fonts. no need for a PNG loader.
              (1.30) - removed GetDefaultFontData(). uses
io.Fonts->GetTextureData*() API to retrieve uncompressed pixels.
                       this sequence:
                           const void* png_data;
                           unsigned int png_size;
                           ImGui::GetDefaultFontData(NULL, NULL, &png_data,
&png_size);
                           // <Copy to GPU>
                       became:
                           unsigned char* pixels;
                           int width, height;
                           io.Fonts->GetTexDataAsRGBA32(&pixels, &width,
&height);
                           // <Copy to GPU>
                           io.Fonts->TexID = (your_texture_identifier);
                       you now have much more flexibility to load multiple TTF
fonts and manage the texture buffer for internal needs.
                       it is now recommended that you sample the font texture
with bilinear interpolation.
              (1.30) - added texture identifier in ImDrawCmd passed to your
render function (we can now render images). make sure to set io.Fonts->TexID.
              (1.30) - removed IO.PixelCenterOffset (unnecessary, can be handled
in user projection matrix)
              (1.30) - removed ImGui::IsItemFocused() in favor of
ImGui::IsItemActive() which handles all widgets
 - 2014/12/10 (1.18) - removed SetNewWindowDefaultPos() in favor of new generic
API SetNextWindowPos(pos, ImGuiSetCondition_FirstUseEver)
 - 2014/11/28 (1.17) - moved IO.Font*** options to inside the IO.Font->
structure (FontYOffset, FontTexUvForWhite, FontBaseScale, FontFallbackGlyph)
 - 2014/11/26 (1.17) - reworked syntax of IMGUI_ONCE_UPON_A_FRAME helper macro
to increase compiler compatibility
 - 2014/11/07 (1.15) - renamed IsHovered() to IsItemHovered()
 - 2014/10/02 (1.14) - renamed IMGUI_INCLUDE_IMGUI_USER_CPP to
IMGUI_INCLUDE_IMGUI_USER_INL and imgui_user.cpp to imgui_user.inl (more IDE
friendly)
 - 2014/09/25 (1.13) - removed 'text_end' parameter from IO.SetClipboardTextFn
(the string is now always zero-terminated for simplicity)
 - 2014/09/24 (1.12) - renamed SetFontScale() to SetWindowFontScale()
 - 2014/09/24 (1.12) - moved IM_MALLOC/IM_REALLOC/IM_FREE preprocessor defines
to IO.MemAllocFn/IO.MemReallocFn/IO.MemFreeFn
 - 2014/08/30 (1.09) - removed IO.FontHeight (now computed automatically)
 - 2014/08/30 (1.09) - moved IMGUI_FONT_TEX_UV_FOR_WHITE preprocessor define to
IO.FontTexUvForWhite
 - 2014/08/28 (1.09) - changed the behavior of IO.PixelCenterOffset following
various rendering fixes


 FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
 ======================================

 Q: How can I help?
 A: - If you are experienced enough with ImGui and with C/C++, look at the todo
list and see how you want/can help!
    - Become a Patron/donate. Convince your company to become a Patron or
provide serious funding for development time.

 Q: How do I update to a newer version of ImGui?
 A: Overwrite the following files:
      imgui.cpp
      imgui.h
      imgui_demo.cpp
      imgui_draw.cpp
      imgui_internal.h
      stb_rect_pack.h
      stb_textedit.h
      stb_truetype.h
    Don't overwrite imconfig.h if you have made modification to your copy.
    Check the "API BREAKING CHANGES" sections for a list of occasional API
breaking changes. If you have a problem with a function, search for its name
    in the code, there will likely be a comment about it. Please report any
issue to the GitHub page!

 Q: What is ImTextureID and how do I display an image?
 A: ImTextureID is a void* used to pass renderer-agnostic texture references
around until it hits your render function.
    ImGui knows nothing about what those bits represent, it just passes them
around. It is up to you to decide what you want the void* to carry!
    It could be an identifier to your OpenGL texture (cast GLuint to void*), a
pointer to your custom engine material (cast MyMaterial* to void*), etc.
    At the end of the chain, your renderer takes this void* to cast it back into
whatever it needs to select a current texture to render.
    Refer to examples applications, where each renderer (in a
imgui_impl_xxxx.cpp file) is treating ImTextureID as a different thing.
    (c++ tip: OpenGL uses integers to identify textures. You can safely store an
integer into a void*, just cast it to void*, don't take it's address!)
    To display a custom image/texture within an ImGui window, you may use
ImGui::Image(), ImGui::ImageButton(), ImDrawList::AddImage() functions.
    ImGui will generate the geometry and draw calls using the ImTextureID that
you passed and which your renderer can use.
    It is your responsibility to get textures uploaded to your GPU.

 Q: I integrated ImGui in my engine and the text or lines are blurry..
 A: In your Render function, try translating your projection matrix by
(0.5f,0.5f) or (0.375f,0.375f).
    Also make sure your orthographic projection matrix and io.DisplaySize
matches your actual framebuffer dimension.

 Q: I integrated ImGui in my engine and some elements are clipping or
disappearing when I move windows around..
 A: Most likely you are mishandling the clipping rectangles in your render
function. Rectangles provided by ImGui are defined as
(x1=left,y1=top,x2=right,y2=bottom) and NOT as (x1,y1,width,height).

 Q: Can I have multiple widgets with the same label? Can I have widget without a
label? (Yes)
 A: Yes. A primer on the use of labels/IDs in ImGui..

   - Elements that are not clickable, such as Text() items don't need an ID.

   - Interactive widgets require state to be carried over multiple frames (most
typically ImGui often needs to remember what is the "active" widget).
     to do so they need a unique ID. unique ID are typically derived from a
string label, an integer index or a pointer.

       Button("OK");        // Label = "OK",     ID = hash of "OK"
       Button("Cancel");    // Label = "Cancel", ID = hash of "Cancel"

   - ID are uniquely scoped within windows, tree nodes, etc. so no conflict can
happen if you have two buttons called "OK" in two different windows
     or in two different locations of a tree.

   - If you have a same ID twice in the same location, you'll have a conflict:

       Button("OK");
       Button("OK");           // ID collision! Both buttons will be treated as
the same.

     Fear not! this is easy to solve and there are many ways to solve it!

   - When passing a label you can optionally specify extra unique ID information
within string itself. This helps solving the simpler collision cases.
     use "##" to pass a complement to the ID that won't be visible to the
end-user:

       Button("Play");         // Label = "Play",   ID = hash of "Play"
       Button("Play##foo1");   // Label = "Play",   ID = hash of "Play##foo1"
(different from above)
       Button("Play##foo2");   // Label = "Play",   ID = hash of "Play##foo2"
(different from above)

   - If you want to completely hide the label, but still need an ID:

       Checkbox("##On", &b);   // Label = "",       ID = hash of "##On" (no
label!)

   - Occasionally/rarely you might want change a label while preserving a
constant ID. This allows you to animate labels.
     For example you may want to include varying information in a window title
bar (and windows are uniquely identified by their ID.. obviously)
     Use "###" to pass a label that isn't part of ID:

       Button("Hello###ID";   // Label = "Hello",  ID = hash of "ID"
       Button("World###ID";   // Label = "World",  ID = hash of "ID" (same as
above)

       sprintf(buf, "My game (%f FPS)###MyGame");
       Begin(buf);            // Variable label,   ID = hash of "MyGame"

   - Use PushID() / PopID() to create scopes and avoid ID conflicts within the
same Window.
     This is the most convenient way of distinguishing ID if you are iterating
and creating many UI elements.
     You can push a pointer, a string or an integer value. Remember that ID are
formed from the concatenation of everything in the ID stack!

       for (int i = 0; i < 100; i++)
       {
         PushID(i);
         Button("Click");   // Label = "Click",  ID = hash of integer + "label"
(unique)
         PopID();
       }

       for (int i = 0; i < 100; i++)
       {
         MyObject* obj = Objects[i];
         PushID(obj);
         Button("Click");   // Label = "Click",  ID = hash of pointer + "label"
(unique)
         PopID();
       }

       for (int i = 0; i < 100; i++)
       {
         MyObject* obj = Objects[i];
         PushID(obj->Name);
         Button("Click");   // Label = "Click",  ID = hash of string + "label"
(unique)
         PopID();
       }

   - More example showing that you can stack multiple prefixes into the ID
stack:

       Button("Click");     // Label = "Click",  ID = hash of "Click"
       PushID("node");
       Button("Click");     // Label = "Click",  ID = hash of "node" + "Click"
         PushID(my_ptr);
           Button("Click"); // Label = "Click",  ID = hash of "node" + ptr +
"Click"
         PopID();
       PopID();

   - Tree nodes implicitly creates a scope for you by calling PushID().

       Button("Click");     // Label = "Click",  ID = hash of "Click"
       if (TreeNode("node"))
       {
         Button("Click");   // Label = "Click",  ID = hash of "node" + "Click"
         TreePop();
       }

   - When working with trees, ID are used to preserve the open/close state of
each tree node.
     Depending on your use cases you may want to use strings, indices or
pointers as ID.
      e.g. when displaying a single object that may change over time (1-1
relationship), using a static string as ID will preserve your node open/closed
state when the targeted object change.
      e.g. when displaying a list of objects, using indices or pointers as ID
will preserve the node open/closed state differently. experiment and see what
makes more sense!

 Q: How can I tell when ImGui wants my mouse/keyboard inputs and when I can pass
them to my application?
 A: You can read the 'io.WantCaptureXXX' flags in the ImGuiIO structure.
Preferably read them after calling ImGui::NewFrame() to avoid those flags
lagging by one frame, but either should be fine.
    When 'io.WantCaptureMouse' or 'io.WantCaptureKeyboard' flags are set you may
want to discard/hide the inputs from the rest of your application.
    When 'io.WantInputsCharacters' is set to may want to notify your OS to popup
an on-screen keyboard, if available.
    ImGui is tracking dragging and widget activity that may occur outside the
boundary of a window, so 'io.WantCaptureMouse' is a more accurate and complete
than testing for ImGui::IsMouseHoveringAnyWindow().
    (Advanced note: text input releases focus on Return 'KeyDown', so the
following Return 'KeyUp' event that your application receive will typically have
'io.WantcaptureKeyboard=false'.
     Depending on your application logic it may or not be inconvenient. You
might want to track which key-downs were for ImGui (e.g. with an array of bool)
and filter out the corresponding key-ups.)

 Q: How can I load a different font than the default? (default is an embedded
version of ProggyClean.ttf, rendered at size 13)
 A: Use the font atlas to load the TTF file you want:

      ImGuiIO& io = ImGui::GetIO();
      io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels);
      io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()

 Q: How can I easily use icons in my application?
 A: The most convenient and practical way is to merge an icon font such as
FontAwesome inside you main font. Then you can refer to icons within your
strings.
    Read 'How can I load multiple fonts?' and the file 'extra_fonts/README.txt'
for instructions.

 Q: How can I load multiple fonts?
 A: Use the font atlas to pack them into a single texture:
    (Read extra_fonts/README.txt and the code in ImFontAtlas for more details.)

      ImGuiIO& io = ImGui::GetIO();
      ImFont* font0 = io.Fonts->AddFontDefault();
      ImFont* font1 = io.Fonts->AddFontFromFileTTF("myfontfile.ttf",
size_in_pixels);
      ImFont* font2 = io.Fonts->AddFontFromFileTTF("myfontfile2.ttf",
size_in_pixels);
      io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()
      // the first loaded font gets used by default
      // use ImGui::PushFont()/ImGui::PopFont() to change the font at runtime

      // Options
      ImFontConfig config;
      config.OversampleH = 3;
      config.OversampleV = 1;
      config.GlyphExtraSpacing.x = 1.0f;
      io.Fonts->LoadFromFileTTF("myfontfile.ttf", size_pixels, &config);

      // Combine multiple fonts into one (e.g. for icon fonts)
      ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
      ImFontConfig config;
      config.MergeMode = true;
      io.Fonts->AddFontDefault();
      io.Fonts->LoadFromFileTTF("fontawesome-webfont.ttf", 16.0f, &config,
ranges); // Merge icon font
      io.Fonts->LoadFromFileTTF("myfontfile.ttf", size_pixels, NULL, &config,
io.Fonts->GetGlyphRangesJapanese()); // Merge japanese glyphs

 Q: How can I display and input non-Latin characters such as Chinese, Japanese,
Korean, Cyrillic?
 A: When loading a font, pass custom Unicode ranges to specify the glyphs to
load.
    All your strings needs to use UTF-8 encoding. Specifying literal in your
source code using a local code page (such as CP-923 for Japanese or CP-1251 for
Cyrillic) will not work.
    In C++11 you can encode a string literal in UTF-8 by using the
uint8_t"hello" syntax. Otherwise you can convert yourself to UTF-8 or load text
data from file already saved as UTF-8.
    You can also try to remap your local codepage characters to their Unicode
codepoint using font->AddRemapChar(), but international users may have problems
reading/editing your source code.

      io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, NULL,
io.Fonts->GetGlyphRangesJapanese());  // Load Japanese characters
      io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()
      io.ImeWindowHandle = MY_HWND;      // To input using Microsoft IME, give
ImGui the hwnd of your application

    As for text input, depends on you passing the right character code to
io.AddInputCharacter(). The example applications do that.

 Q: How can I use the drawing facilities without an ImGui window? (using
ImDrawList API)
 A: The easiest way is to create a dummy window. Call Begin() with
NoTitleBar|NoResize|NoMove|NoScrollbar|NoSavedSettings|NoInputs flag, zero
background alpha,
    then retrieve the ImDrawList* via GetWindowDrawList() and draw to it in any
way you like.

 - tip: the construct 'IMGUI_ONCE_UPON_A_FRAME { ... }' will run the block of
code only once a frame. You can use it to quickly add custom UI in the middle of
a deep nested inner loop in your code.
 - tip: you can create widgets without a Begin()/End() block, they will go in an
implicit window called "Debug"
 - tip: you can call Begin() multiple times with the same name during the same
frame, it will keep appending to the same window. this is also useful to set
yourself in the context of another window (to get/set other settings)
 - tip: you can call Render() multiple times (e.g for VR renders).
 - tip: call and read the ShowTestWindow() code in imgui_demo.cpp for more
example of how to use ImGui!


 ISSUES & TODO-LIST
 ==================
 Issue numbers (#) refer to github issues listed at
https://github.com/ocornut/imgui/issues
 The list below consist mostly of ideas noted down before they are
requested/discussed by users (at which point it usually moves to the github)

 - doc: add a proper documentation+regression testing system (#435)
 - window: add a way for very transient windows (non-saved, temporary overlay
over hundreds of objects) to "clean" up from the global window list. perhaps a
lightweight explicit cleanup pass.
 - window: calling SetNextWindowSize() every frame with <= 0 doesn't do
anything, may be useful to allow (particularly when used for a single axis)
(#690)
 - window: auto-fit feedback loop when user relies on any dynamic layout (window
width multiplier, column) appears weird to end-user. clarify.
 - window: allow resizing of child windows (possibly given min/max for each
axis?)
 - window: background options for child windows, border option (disable
rounding)
 - window: add a way to clear an existing window instead of appending (e.g. for
tooltip override using a consistent api rather than the deferred tooltip)
 - window: resizing from any sides? + mouse cursor directives for app.
!- window: begin with *p_open == false should return false.
 - window: get size/pos helpers given names (see discussion in #249)
 - window: a collapsed window can be stuck behind the main menu bar?
 - window: when window is small, prioritize resize button over close button.
 - window: detect extra End() call that pop the "Debug" window out and assert at
call site instead of later.
 - window/tooltip: allow to set the width of a tooltip to allow TextWrapped()
etc. while keeping the height automatic.
 - window: increase minimum size of a window with menus or fix the menu
rendering so that it doesn't look odd.
 - draw-list: maintaining bounding box per command would allow to merge draw
command when clipping isn't relied on (typical non-scrolling window or
non-overflowing column would merge with previous command).
!- scrolling: allow immediately effective change of scroll if we haven't
appended items yet
 - splitter/separator: formalize the splitter idiom into an official api (we
want to handle n-way split) (#319)
 - widgets: display mode: widget-label, label-widget (aligned on column or using
fixed size), label-newline-tab-widget etc.
 - widgets: clean up widgets internal toward exposing everything.
 - widgets: add disabled and read-only modes (#211)
 - main: considering adding an Init() function? some constructs are awkward in
the implementation because of the lack of them.
!- main: make it so that a frame with no window registered won't refocus every
window on subsequent frames (~bump LastFrameActive of all windows).
 - main: IsItemHovered() make it more consistent for various type of widgets,
widgets with multiple components, etc. also effectively IsHovered() region
sometimes differs from hot region, e.g tree nodes
 - main: IsItemHovered() info stored in a stack? so that 'if TreeNode() { Text;
TreePop; } if IsHovered' return the hover state of the TreeNode?
 - input text: clean up the mess caused by converting UTF-8 <> wchar. the code
is rather inefficient right now and super fragile.
 - input text: reorganize event handling, allow CharFilter to modify buffers,
allow multiple events? (#541)
 - input text: expose CursorPos in char filter event (#816)
 - input text: flag to disable live update of the user buffer (also applies to
float/int text input)
 - input text: resize behavior - field could stretch when being edited? hover
tooltip shows more text?
 - input text: add ImGuiInputTextFlags_EnterToApply? (off #218)
 - input text: add discard flag (e.g. ImGuiInputTextFlags_DiscardActiveBuffer)
or make it easier to clear active focus for text replacement during edition
(#725)
 - input text multi-line: don't directly call AddText() which does an
unnecessary vertex reserve for character count prior to clipping. and/or more
line-based clipping to AddText(). and/or reorganize TextUnformatted/RenderText
for more efficiency for large text (e.g TextUnformatted could clip and log
separately, etc).
 - input text multi-line: way to dynamically grow the buffer without forcing the
user to initially allocate for worse case (follow up on #200)
 - input text multi-line: line numbers? status bar? (follow up on #200)
 - input text multi-line: behave better when user changes input buffer while
editing is active (even though it is illegal behavior). namely, the change of
buffer can create a scrollbar glitch (#725)
 - input text: allow centering/positioning text so that ctrl+clicking Drag or
Slider keeps the textual value at the same pixel position.
 - input number: optional range min/max for Input*() functions
 - input number: holding [-]/[+] buttons could increase the step speed
non-linearly (or user-controlled)
 - input number: use mouse wheel to step up/down
 - input number: applying arithmetics ops (+,-,*,/) messes up with text edit
undo stack.
 - button: provide a button that looks framed.
 - text: proper alignment options
 - image/image button: misalignment on padded/bordered button?
 - image/image button: parameters are confusing, image() has tint_col,border_col
whereas imagebutton() has bg_col/tint_col. Even thou they are different
parameters ordering could be more consistent. can we fix that?
 - layout: horizontal layout helper (#97)
 - layout: horizontal flow until no space left (#404)
 - layout: more generic alignment state (left/right/centered) for single items?
 - layout: clean up the InputFloatN/SliderFloatN/ColorEdit4 layout code. item
width should include frame padding.
 - layout: BeginGroup() needs a border option.
 - columns: declare column set (each column: fixed size, %, fill, distribute
default size among fills) (#513, #125)
 - columns: add a conditional parameter to SetColumnOffset() (#513, #125)
 - columns: separator function or parameter that works within the column
(currently Separator() bypass all columns) (#125)
 - columns: columns header to act as button (~sort op) and allow resize/reorder
(#513, #125)
 - columns: user specify columns size (#513, #125)
 - columns: flag to add horizontal separator above/below?
 - columns/layout: setup minimum line height (equivalent of automatically
calling AlignFirstTextHeightToWidgets)
 - combo: sparse combo boxes (via function call?) / iterators
 - combo: contents should extends to fit label if combo widget is small
 - combo/listbox: keyboard control. need InputText-like non-active focus + key
handling. considering keyboard for custom listbox (pr #203)
 - listbox: multiple selection
 - listbox: user may want to initial scroll to focus on the one selected value?
 - listbox: keyboard navigation.
 - listbox: scrolling should track modified selection.
!- popups/menus: clarify usage of popups id, how MenuItem/Selectable closing
parent popups affects the ID, etc. this is quite fishy needs improvement! (#331,
#402)
 - popups: add variant using global identifier similar to Begin/End (#402)
 - popups: border options. richer api like BeginChild() perhaps? (#197)
 - tooltip: tooltip that doesn't fit in entire screen seems to lose their "last
preferred button" and may teleport when moving mouse
 - menus: local shortcuts, global shortcuts (#456, #126)
 - menus: icons
 - menus: menubars: some sort of priority / effect of main menu-bar on desktop
size?
 - menus: calling BeginMenu() twice with a same name doesn't seem to append
nicely
 - statusbar: add a per-window status bar helper similar to what menubar does.
 - tabs (#261, #351)
 - separator: separator on the initial position of a window is not visible
(cursorpos.y <= clippos.y)
!- color: the color helpers/typing is a mess and needs sorting out.
 - color: add a better color picker (#346)
 - node/graph editor (#306)
 - pie menus patterns (#434)
 - drag'n drop, dragging helpers (carry dragging info, visualize drag source
before clicking, drop target, etc.) (#143, #479)
 - plot: PlotLines() should use the polygon-stroke facilities (currently issues
with averaging normals)
 - plot: make it easier for user to draw extra stuff into the graph (e.g: draw
basis, highlight certain points, 2d plots, multiple plots)
 - plot: "smooth" automatic scale over time, user give an input 0.0(full user
scale) 1.0(full derived from value)
 - plot: add a helper e.g. Plot(char* label, float value, float time_span=2.0f)
that stores values and Plot them for you - probably another function name.
and/or automatically allow to plot ANY displayed value (more reliance on stable
ID)
 - slider: allow using the [-]/[+] buttons used by InputFloat()/InputInt()
 - slider: initial absolute click is imprecise. change to relative movement
slider (same as scrollbar).
 - slider: add dragging-based widgets to edit values with mouse (on 2 axises),
saving screen real-estate.
 - slider: tint background based on value (e.g. v_min -> v_max, or use 0.0f
either side of the sign)
 - slider & drag: int data passing through a float
 - drag float: up/down axis
 - drag float: added leeway on edge (e.g. a few invisible steps past the clamp
limits)
 - tree node / optimization: avoid formatting when clipped.
 - tree node: tree-node/header right-most side doesn't take account of
horizontal scrolling.
 - tree node: add treenode/treepush int variants? not there because (void*) cast
from int warns on some platforms/settings?
 - tree node: try to apply scrolling at time of TreePop() if node was just
opened and end of node is past scrolling limits?
 - tree node / selectable render mismatch which is visible if you use them both
next to each other (e.g. cf. property viewer)
 - tree node: tweak color scheme to distinguish headers from selected tree node
(#581)
 - textwrapped: figure out better way to use TextWrapped() in an always
auto-resize context (tooltip, etc.) (#249)
 - settings: write more decent code to allow saving/loading new fields
 - settings: api for per-tool simple persistent data (bool,int,float,columns
sizes,etc.) in .ini file
 - style: add window shadows.
 - style/optimization: store rounded corners in texture to use 1 quad per corner
(filled and wireframe) to lower the cost of rounding.
 - style: color-box not always square?
 - style: a concept of "compact style" that the end-user can easily rely on
(e.g. PushStyleCompact()?) that maps to other settings? avoid implementing
duplicate helpers such as SmallCheckbox(), etc.
 - style: try to make PushStyleVar() more robust to incorrect parameters (to be
more friendly to edit & continues situation).
 - style: global scale setting.
 - style: WindowPadding needs to be EVEN needs the 0.5 multiplier probably have
a subtle effect on clip rectangle
 - text: simple markup language for color change?
 - font: dynamic font atlas to avoid baking huge ranges into bitmap and make
scaling easier.
 - font: small opt: for monospace font (like the defalt one) we can trim
IndexXAdvance as long as trailing value is == FallbackXAdvance
 - font: add support for kerning, probably optional. perhaps default to
(32..128)^2 matrix ~ 36KB then hash fallback.
 - font: add a simpler CalcTextSizeA() api? current one ok but not welcome if
user needs to call it directly (without going through ImGui::CalcTextSize)
 - font: fix AddRemapChar() to work before font has been built.
 - log: LogButtons() options for specifying depth and/or hiding depth slider
 - log: have more control over the log scope (e.g. stop logging when leaving
current tree node scope)
 - log: be able to log anything (e.g. right-click on a window/tree-node, shows
context menu? log into tty/file/clipboard)
 - log: let user copy any window content to clipboard easily (CTRL+C on windows?
while moving it? context menu?). code is commented because it fails with
multiple Begin/End pairs.
 - filters: set a current filter that tree node can automatically query to hide
themselves
 - filters: handle wildcards (with implicit leading/trailing *), regexps
 - shortcuts: add a shortcut api, e.g. parse "&Save" and/or "Save (CTRL+S)",
pass in to widgets or provide simple ways to use (button=activate, input=focus)
!- keyboard: tooltip & combo boxes are messing up / not honoring keyboard
tabbing
 - keyboard: full keyboard navigation and focus. (#323)
 - focus: preserve ActiveId/focus stack state, e.g. when opening a menu and
close it, previously selected InputText() focus gets restored (#622)
 - focus: SetKeyboardFocusHere() on with >= 0 offset could be done on same frame
(else latch and modulate on beginning of next frame)
 - input: rework IO system to be able to pass actual ordered/timestamped events.
(~#335, #71)
 - input: allow to decide and pass explicit double-clicks (e.g. for windows by
the CS_DBLCLKS style).
 - input: support track pad style scrolling & slider edit.
 - misc: provide a way to compile out the entire implementation while providing
a dummy API (e.g. #define IMGUI_DUMMY_IMPL)
 - misc: double-clicking on title bar to minimize isn't consistent, perhaps move
to single-click on left-most collapse icon?
 - misc: provide HoveredTime and ActivatedTime to ease the creation of
animations.
 - style editor: have a more global HSV setter (e.g. alter hue on all elements).
consider replacing active/hovered by offset in HSV space? (#438)
 - style editor: color child window height expressed in multiple of line height.
 - remote: make a system like RemoteImGui first-class citizen/project (#75)
 - drawlist: move Font, FontSize, FontTexUvWhitePixel inside ImDrawList and make
it self-contained (apart from drawing settings?)
 - drawlist: end-user probably can't call Clear() directly because we expect a
texture to be pushed in the stack.
 - examples: directx9: save/restore device state more thoroughly.
 - examples: window minimize, maximize (#583)
 - optimization: add a flag to disable most of rendering, for the case where the
user expect to skip it (#335)
 - optimization: use another hash function than crc32, e.g. FNV1a
 - optimization/render: merge command-lists with same clip-rect into one even if
they aren't sequential? (as long as in-between clip rectangle don't overlap)?
 - optimization: turn some the various stack vectors into statically-sized
arrays
 - optimization: better clipping for multi-component widgets
*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

<<<<<<< HEAD
#include <limits.h>                        // INT_MIN, INT_MAX
#if defined(_MSC_VER) && _MSC_VER <= 1500  // MSVC 2008 or earlier
#include <stddef.h>                        // intptr_t
#else
#include <stdint.h>  // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4127)  // condition expression is constant
#pragma warning(disable : 4505)  // unreferenced local function has been removed
                                 // (stb stuff)
#pragma warning(disable : 4996)  // 'This function or variable may be unsafe':
                                 // strcpy, strdup, sprintf, vsnprintf, sscanf,
                                 // fopen
=======
#include <limits.h>                       // INT_MIN, INT_MAX
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>                       // intptr_t
#else
#include <stdint.h> // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4127) // condition expression is constant
#pragma warning(disable : 4505) // unreferenced local function has been removed
                                // (stb stuff)
#pragma warning(disable : 4996) // 'This function or variable may be unsafe':
                                // strcpy, strdup, sprintf, vsnprintf, sscanf,
                                // fopen
>>>>>>> master
#endif

// Clang warnings with -Weverything
#ifdef __clang__
<<<<<<< HEAD
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast
                                                            // // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is
                                                            // unsafe   // storing and comparing against same constants
                                                            // (typically 0.0f) is ok.
#pragma clang diagnostic ignored "-Wformat-nonliteral"      // warning : format string is not a string literal
                                                            // // passing non-literal to vsnformat(). yes, user
                                                            // passing incorrect format strings can crash the
                                                            // code.
#pragma clang diagnostic ignored "-Wexit-time-destructors"  // warning : declaration requires an exit-time
                                                            // destructor       // exit-time destruction order
                                                            // is undefined. if MemFree() leads to users code
                                                            // that has been disabled before exit it might
                                                            // cause problems. ImGui coding style welcomes
                                                            // static/globals.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global
                                                            // destructor           // similar to above, not
                                                            // sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness //
#pragma clang diagnostic ignored "-Wformat-pedantic"        // warning : format specifies type 'void *' but the
                                                            // argument has type 'xxxx *' // unreasonable, would
                                                            // lead to casting every %p arg to void*. probably
                                                            // enabled by -pedantic.
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"  // warning : cast to 'void *' from smaller
                                                               // integer type 'int' //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"      // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"  // warning: cast to
// pointer from integer
// of different size
#pragma GCC diagnostic ignored "-Wformat"  // warning: format '%p' expects argument of type 'void*', but
// argument 6 has type 'ImGuiWindow*'
#pragma GCC diagnostic ignored "-Wdouble-promotion"  // warning: implicit conversion from 'float' to
// 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"  // warning: conversion to 'xxxx' from 'xxxx' may alter its
// value
#pragma GCC diagnostic ignored "-Wcast-qual"  // warning: cast from type 'xxxx'
=======
#pragma clang diagnostic ignored                                               \
    "-Wold-style-cast" // warning : use of old-style cast
                       // // yes, they are more terse.
#pragma clang diagnostic ignored                                               \
    "-Wfloat-equal" // warning : comparing floating point with == or != is
                    // unsafe   // storing and comparing against same constants
                    // (typically 0.0f) is ok.
#pragma clang diagnostic ignored                                               \
    "-Wformat-nonliteral" // warning : format string is not a string literal
                          // // passing non-literal to vsnformat(). yes, user
                          // passing incorrect format strings can crash the
                          // code.
#pragma clang diagnostic ignored                                               \
    "-Wexit-time-destructors" // warning : declaration requires an exit-time
                              // destructor       // exit-time destruction order
                              // is undefined. if MemFree() leads to users code
                              // that has been disabled before exit it might
                              // cause problems. ImGui coding style welcomes
                              // static/globals.
#pragma clang diagnostic ignored                                               \
    "-Wglobal-constructors" // warning : declaration requires a global
                            // destructor           // similar to above, not
                            // sure what the exact difference it.
#pragma clang diagnostic ignored                                               \
    "-Wsign-conversion" // warning : implicit conversion changes signedness //
#pragma clang diagnostic ignored                                               \
    "-Wformat-pedantic" // warning : format specifies type 'void *' but the
                        // argument has type 'xxxx *' // unreasonable, would
                        // lead to casting every %p arg to void*. probably
                        // enabled by -pedantic.
#pragma clang diagnostic ignored                                               \
    "-Wint-to-void-pointer-cast" // warning : cast to 'void *' from smaller
                                 // integer type 'int' //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored                                                 \
    "-Wunused-function" // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" // warning: cast to
// pointer from integer
// of different size
#pragma GCC diagnostic ignored                                                 \
    "-Wformat" // warning: format '%p' expects argument of type 'void*', but
// argument 6 has type 'ImGuiWindow*'
#pragma GCC diagnostic ignored                                                 \
    "-Wdouble-promotion" // warning: implicit conversion from 'float' to
// 'double' when passing argument to function
#pragma GCC diagnostic ignored                                                 \
    "-Wconversion" // warning: conversion to 'xxxx' from 'xxxx' may alter its
// value
#pragma GCC diagnostic ignored "-Wcast-qual" // warning: cast from type 'xxxx'
>>>>>>> master
// to type 'xxxx' casts away
// qualifiers
#endif

//-------------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------------

<<<<<<< HEAD
static void LogRenderedText(const ImVec2& ref_pos, const char* text, const char* text_end = NULL);
=======
static void LogRenderedText(const ImVec2 &ref_pos, const char *text,
                            const char *text_end = NULL);
>>>>>>> master

static void PushMultiItemsWidths(int components, float w_full = 0.0f);
static float GetDraggedColumnOffset(int column_index);

static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true);

<<<<<<< HEAD
static ImFont* GetDefaultFont();
static void SetCurrentFont(ImFont* font);
static void SetCurrentWindow(ImGuiWindow* window);
static void SetWindowScrollY(ImGuiWindow* window, float new_scroll_y);
static void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiSetCond cond);
static void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiSetCond cond);
static void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiSetCond cond);
static ImGuiWindow* FindHoveredWindow(ImVec2 pos, bool excluding_childs);
static ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags);
static inline bool IsWindowContentHoverable(ImGuiWindow* window);
static void ClearSetNextWindowData();
static void CheckStacksSize(ImGuiWindow* window, bool write);
static void Scrollbar(ImGuiWindow* window, bool horizontal);

static void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list);
static void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window);
static void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window);

static ImGuiIniData* FindWindowSettings(const char* name);
static ImGuiIniData* AddWindowSettings(const char* name);
static void LoadIniSettingsFromDisk(const char* ini_filename);
static void SaveIniSettingsToDisk(const char* ini_filename);
=======
static ImFont *GetDefaultFont();
static void SetCurrentFont(ImFont *font);
static void SetCurrentWindow(ImGuiWindow *window);
static void SetWindowScrollY(ImGuiWindow *window, float new_scroll_y);
static void SetWindowPos(ImGuiWindow *window, const ImVec2 &pos,
                         ImGuiSetCond cond);
static void SetWindowSize(ImGuiWindow *window, const ImVec2 &size,
                          ImGuiSetCond cond);
static void SetWindowCollapsed(ImGuiWindow *window, bool collapsed,
                               ImGuiSetCond cond);
static ImGuiWindow *FindHoveredWindow(ImVec2 pos, bool excluding_childs);
static ImGuiWindow *CreateNewWindow(const char *name, ImVec2 size,
                                    ImGuiWindowFlags flags);
static inline bool IsWindowContentHoverable(ImGuiWindow *window);
static void ClearSetNextWindowData();
static void CheckStacksSize(ImGuiWindow *window, bool write);
static void Scrollbar(ImGuiWindow *window, bool horizontal);

static void AddDrawListToRenderList(ImVector<ImDrawList *> &out_render_list,
                                    ImDrawList *draw_list);
static void AddWindowToRenderList(ImVector<ImDrawList *> &out_render_list,
                                  ImGuiWindow *window);
static void AddWindowToSortedBuffer(ImVector<ImGuiWindow *> &out_sorted_windows,
                                    ImGuiWindow *window);

static ImGuiIniData *FindWindowSettings(const char *name);
static ImGuiIniData *AddWindowSettings(const char *name);
static void LoadIniSettingsFromDisk(const char *ini_filename);
static void SaveIniSettingsToDisk(const char *ini_filename);
>>>>>>> master
static void MarkIniSettingsDirty();

static void PushColumnClipRect(int column_index = -1);
static ImRect GetVisibleRect();

<<<<<<< HEAD
static bool BeginPopupEx(const char* str_id, ImGuiWindowFlags extra_flags);
=======
static bool BeginPopupEx(const char *str_id, ImGuiWindowFlags extra_flags);
>>>>>>> master
static void CloseInactivePopups();
static void ClosePopupToLevel(int remaining);
static void ClosePopup(ImGuiID id);
static bool IsPopupOpen(ImGuiID id);
<<<<<<< HEAD
static ImGuiWindow* GetFrontMostModalRootWindow();
static ImVec2       FindBestPopupWindowPos(
    const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& rect_to_avoid);

static bool InputTextFilterCharacter(
    unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data);
static int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
static ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end,
    const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);

static inline void DataTypeFormatString(
    ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size);
static inline void DataTypeFormatString(
    ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size);
static void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2);
static bool DataTypeApplyOpFromText(
    const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format);
=======
static ImGuiWindow *GetFrontMostModalRootWindow();
static ImVec2 FindBestPopupWindowPos(const ImVec2 &base_pos, const ImVec2 &size,
                                     int *last_dir,
                                     const ImRect &rect_to_avoid);

static bool InputTextFilterCharacter(unsigned int *p_char,
                                     ImGuiInputTextFlags flags,
                                     ImGuiTextEditCallback callback,
                                     void *user_data);
static int InputTextCalcTextLenAndLineCount(const char *text_begin,
                                            const char **out_text_end);
static ImVec2 InputTextCalcTextSizeW(const ImWchar *text_begin,
                                     const ImWchar *text_end,
                                     const ImWchar **remaining = NULL,
                                     ImVec2 *out_offset = NULL,
                                     bool stop_on_new_line = false);

static inline void DataTypeFormatString(ImGuiDataType data_type, void *data_ptr,
                                        const char *display_format, char *buf,
                                        int buf_size);
static inline void DataTypeFormatString(ImGuiDataType data_type, void *data_ptr,
                                        int decimal_precision, char *buf,
                                        int buf_size);
static void DataTypeApplyOp(ImGuiDataType data_type, int op, void *value1,
                            const void *value2);
static bool DataTypeApplyOpFromText(const char *buf,
                                    const char *initial_value_buf,
                                    ImGuiDataType data_type, void *data_ptr,
                                    const char *scalar_format);
>>>>>>> master

//-----------------------------------------------------------------------------
// Platform dependent default implementations
//-----------------------------------------------------------------------------

<<<<<<< HEAD
static const char* GetClipboardTextFn_DefaultImpl(void* user_data);
static void SetClipboardTextFn_DefaultImpl(void* user_data, const char* text);
=======
static const char *GetClipboardTextFn_DefaultImpl(void *user_data);
static void SetClipboardTextFn_DefaultImpl(void *user_data, const char *text);
>>>>>>> master
static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y);

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------

// Default font atlas storage .
// New contexts always point by default to this font atlas. It can be changed by
// reassigning the GetIO().Fonts variable.
static ImFontAtlas GImDefaultFontAtlas;

// Default context storage + current context pointer.
// Implicitely used by all ImGui functions. Always assumed to be != NULL. Change
// to a different context by calling ImGui::SetCurrentContext()
// ImGui is currently not thread-safe because of this variable. If you want
// thread-safety to allow N threads to access N different contexts, you might
// work around it by:
// - Having multiple instances of the ImGui code compiled inside different
// namespace (easiest/safest, if you have a finite number of contexts)
// - or: Changing this variable to be TLS. You may #define GImGui in imconfig.h
// for further custom hackery. Future development aim to make this context
// pointer explicit to all calls. Also read
// https://github.com/ocornut/imgui/issues/586
#ifndef GImGui
static ImGuiContext GImDefaultContext;
<<<<<<< HEAD
ImGuiContext*       GImGui = &GImDefaultContext;
=======
ImGuiContext *GImGui = &GImDefaultContext;
>>>>>>> master
#endif

//-----------------------------------------------------------------------------
// User facing structures
//-----------------------------------------------------------------------------

<<<<<<< HEAD
ImGuiStyle::ImGuiStyle()
{
    Alpha          = 1.0f;                      // Global alpha applies to everything in ImGui
    WindowPadding  = ImVec2(8, 8);              // Padding within a window
    WindowMinSize  = ImVec2(32, 32);            // Minimum window size
    WindowRounding = 9.0f;                      // Radius of window corners rounding. Set to 0.0f to
                                                // have rectangular windows
    WindowTitleAlign    = ImVec2(0.0f, 0.5f);   // Alignment for title bar text
    ChildWindowRounding = 0.0f;                 // Radius of child window corners rounding. Set to
                                                // 0.0f to have rectangular child windows
    FramePadding  = ImVec2(4, 3);               // Padding within a framed rectangle (used by most widgets)
    FrameRounding = 0.0f;                       // Radius of frame corners rounding. Set to 0.0f to have
                                                // rectangular frames (used by most widgets).
    ItemSpacing      = ImVec2(8, 4);            // Horizontal and vertical spacing between widgets/lines
    ItemInnerSpacing = ImVec2(4, 4);            // Horizontal and vertical spacing between
                                                // within elements of a composed widget (e.g.
                                                // a slider and its label)
    TouchExtraPadding = ImVec2(0, 0);           // Expand reactive bounding box for touch-based system where
                                                // touch position is not accurate enough. Unfortunately we
                                                // don't sort widgets so priority on overlap will always be
                                                // given to the first widget. So don't grow this too much!
    IndentSpacing = 21.0f;                      // Horizontal spacing when e.g. entering a tree node.
                                                // Generally == (FontSize + FramePadding.x*2).
    ColumnsMinSpacing = 6.0f;                   // Minimum horizontal spacing between two columns
    ScrollbarSize     = 16.0f;                  // Width of the vertical scrollbar, Height of the
                                                // horizontal scrollbar
    ScrollbarRounding = 9.0f;                   // Radius of grab corners rounding for scrollbar
    GrabMinSize       = 10.0f;                  // Minimum width/height of a grab box for slider/scrollbar
    GrabRounding      = 0.0f;                   // Radius of grabs corners rounding. Set to 0.0f to have
                                                // rectangular slider grabs.
    ButtonTextAlign      = ImVec2(0.5f, 0.5f);  // Alignment of button text when button is larger than text.
    DisplayWindowPadding = ImVec2(22, 22);      // Window positions are clamped to be
                                                // visible within the display area by
                                                // at least this amount. Only covers
                                                // regular windows.
    DisplaySafeAreaPadding = ImVec2(4, 4);      // If you cannot see the edge of your screen (e.g. on a TV)
                                                // increase the safe area padding. Covers popups/tooltips as
                                                // well regular windows.
    AntiAliasedLines = true;                    // Enable anti-aliasing on lines/borders. Disable if
                                                // you are really short on CPU/GPU.
    AntiAliasedShapes = true;                   // Enable anti-aliasing on filled shapes (rounded
                                                // rectangles, circles, etc.)
    CurveTessellationTol = 1.25f;               // Tessellation tolerance. Decrease for highly
                                                // tessellated curves (higher quality, more
                                                // polygons), increase to reduce quality.

    Colors[ImGuiCol_Text]          = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    Colors[ImGuiCol_TextDisabled]  = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    Colors[ImGuiCol_WindowBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg]       = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    Colors[ImGuiCol_Border]        = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    Colors[ImGuiCol_BorderShadow]  = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_FrameBg]       = ImVec4(0.80f,
        0.80f,
        0.80f,
        0.30f);  // Background of checkbox, radio button, plot, slider, text input
    Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    Colors[ImGuiCol_TitleBg]              = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
    Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
    Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
    Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
    Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
    Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
    Colors[ImGuiCol_CheckMark]            = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    Colors[ImGuiCol_SliderGrab]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    Colors[ImGuiCol_Button]               = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
    Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
    Colors[ImGuiCol_ButtonActive]         = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    Colors[ImGuiCol_Header]               = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
    Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
    Colors[ImGuiCol_HeaderActive]         = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
    Colors[ImGuiCol_Column]               = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    Colors[ImGuiCol_ColumnActive]         = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    Colors[ImGuiCol_CloseButton]          = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
    Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
    Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
=======
ImGuiStyle::ImGuiStyle() {
  Alpha = 1.0f;                   // Global alpha applies to everything in ImGui
  WindowPadding = ImVec2(8, 8);   // Padding within a window
  WindowMinSize = ImVec2(32, 32); // Minimum window size
  WindowRounding = 9.0f; // Radius of window corners rounding. Set to 0.0f to
                         // have rectangular windows
  WindowTitleAlign = ImVec2(0.0f, 0.5f); // Alignment for title bar text
  ChildWindowRounding = 0.0f; // Radius of child window corners rounding. Set to
                              // 0.0f to have rectangular child windows
  FramePadding =
      ImVec2(4, 3); // Padding within a framed rectangle (used by most widgets)
  FrameRounding = 0.0f; // Radius of frame corners rounding. Set to 0.0f to have
                        // rectangular frames (used by most widgets).
  ItemSpacing =
      ImVec2(8, 4); // Horizontal and vertical spacing between widgets/lines
  ItemInnerSpacing = ImVec2(4, 4); // Horizontal and vertical spacing between
                                   // within elements of a composed widget (e.g.
                                   // a slider and its label)
  TouchExtraPadding =
      ImVec2(0, 0); // Expand reactive bounding box for touch-based system where
                    // touch position is not accurate enough. Unfortunately we
                    // don't sort widgets so priority on overlap will always be
                    // given to the first widget. So don't grow this too much!
  IndentSpacing = 21.0f; // Horizontal spacing when e.g. entering a tree node.
                         // Generally == (FontSize + FramePadding.x*2).
  ColumnsMinSpacing = 6.0f; // Minimum horizontal spacing between two columns
  ScrollbarSize = 16.0f;    // Width of the vertical scrollbar, Height of the
                            // horizontal scrollbar
  ScrollbarRounding = 9.0f; // Radius of grab corners rounding for scrollbar
  GrabMinSize =
      10.0f; // Minimum width/height of a grab box for slider/scrollbar
  GrabRounding = 0.0f; // Radius of grabs corners rounding. Set to 0.0f to have
                       // rectangular slider grabs.
  ButtonTextAlign = ImVec2(
      0.5f, 0.5f); // Alignment of button text when button is larger than text.
  DisplayWindowPadding = ImVec2(22, 22); // Window positions are clamped to be
                                         // visible within the display area by
                                         // at least this amount. Only covers
                                         // regular windows.
  DisplaySafeAreaPadding =
      ImVec2(4, 4); // If you cannot see the edge of your screen (e.g. on a TV)
                    // increase the safe area padding. Covers popups/tooltips as
                    // well regular windows.
  AntiAliasedLines = true;  // Enable anti-aliasing on lines/borders. Disable if
                            // you are really short on CPU/GPU.
  AntiAliasedShapes = true; // Enable anti-aliasing on filled shapes (rounded
                            // rectangles, circles, etc.)
  CurveTessellationTol = 1.25f; // Tessellation tolerance. Decrease for highly
                                // tessellated curves (higher quality, more
                                // polygons), increase to reduce quality.

  Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
  Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
  Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
  Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  Colors[ImGuiCol_FrameBg] = ImVec4(
      0.80f, 0.80f, 0.80f,
      0.30f); // Background of checkbox, radio button, plot, slider, text input
  Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
  Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
  Colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
  Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
  Colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
  Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
  Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
  Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
  Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
  Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
  Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
  Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
  Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
  Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
  Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
  Colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
  Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
  Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
  Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
  Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
  Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
  Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
  Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
  Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
  Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
  Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
  Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
  Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
  Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
>>>>>>> master
}

ImGuiIO::ImGuiIO() {
  // Most fields are initialized with zero
  memset(this, 0, sizeof(*this));

<<<<<<< HEAD
    DisplaySize             = ImVec2(-1.0f, -1.0f);
    DeltaTime               = 1.0f / 60.0f;
    IniSavingRate           = 5.0f;
    IniFilename             = NULL;
    LogFilename             = NULL;
    Fonts                   = &GImDefaultFontAtlas;
    FontGlobalScale         = 1.0f;
    FontDefault             = NULL;
    DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    MousePos                = ImVec2(-1, -1);
    MousePosPrev            = ImVec2(-1, -1);
    MouseDoubleClickTime    = 0.30f;
    MouseDoubleClickMaxDist = 6.0f;
    MouseDragThreshold      = 6.0f;
    for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++) MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++) KeysDownDuration[i] = KeysDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < ImGuiKey_COUNT; i++) KeyMap[i] = -1;
    KeyRepeatDelay                                     = 0.250f;
    KeyRepeatRate                                      = 0.050f;
    UserData                                           = NULL;

    // User functions
    RenderDrawListsFn  = NULL;
    MemAllocFn         = kalloc;
    MemFreeFn          = kfree;
    GetClipboardTextFn = GetClipboardTextFn_DefaultImpl;  // Platform dependent
                                                          // default
                                                          // implementations
    SetClipboardTextFn     = SetClipboardTextFn_DefaultImpl;
    ClipboardUserData      = NULL;
    ImeSetInputScreenPosFn = ImeSetInputScreenPosFn_DefaultImpl;
=======
  DisplaySize = ImVec2(-1.0f, -1.0f);
  DeltaTime = 1.0f / 60.0f;
  IniSavingRate = 5.0f;
  IniFilename = NULL;
  LogFilename = NULL;
  Fonts = &GImDefaultFontAtlas;
  FontGlobalScale = 1.0f;
  FontDefault = NULL;
  DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  MousePos = ImVec2(-1, -1);
  MousePosPrev = ImVec2(-1, -1);
  MouseDoubleClickTime = 0.30f;
  MouseDoubleClickMaxDist = 6.0f;
  MouseDragThreshold = 6.0f;
  for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++)
    MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
  for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++)
    KeysDownDuration[i] = KeysDownDurationPrev[i] = -1.0f;
  for (int i = 0; i < ImGuiKey_COUNT; i++)
    KeyMap[i] = -1;
  KeyRepeatDelay = 0.250f;
  KeyRepeatRate = 0.050f;
  UserData = NULL;

  // User functions
  RenderDrawListsFn = NULL;
  MemAllocFn = kalloc;
  MemFreeFn = kfree;
  GetClipboardTextFn = GetClipboardTextFn_DefaultImpl; // Platform dependent
                                                       // default
                                                       // implementations
  SetClipboardTextFn = SetClipboardTextFn_DefaultImpl;
  ClipboardUserData = NULL;
  ImeSetInputScreenPosFn = ImeSetInputScreenPosFn_DefaultImpl;
>>>>>>> master

// Set OS X style defaults based on __APPLE__ compile time flag
#ifdef __APPLE__
  OSXBehaviors = true;
#endif
}

// Pass in translated ASCII characters for text input.
// - with glfw you can get those from the callback set in glfwSetCharCallback()
// - on Windows you can get those using ToAscii+keyboard state, or via the
// WM_CHAR message
<<<<<<< HEAD
void ImGuiIO::AddInputCharacter(ImWchar c)
{
    const int n = ImStrlenW(InputCharacters);
    if (n + 1 < IM_ARRAYSIZE(InputCharacters)) {
        InputCharacters[n]     = c;
        InputCharacters[n + 1] = '\0';
    }
}

void ImGuiIO::AddInputCharactersUTF8(const char* utf8_chars)
{
    // We can't pass more wchars than ImGuiIO::InputCharacters[] can hold so don't
    // convert more
    const int wchars_buf_len = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar);
    ImWchar   wchars[wchars_buf_len];
    ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL);
    for (int i = 0; i < wchars_buf_len && wchars[i] != 0; i++) AddInputCharacter(wchars[i]);
=======
void ImGuiIO::AddInputCharacter(ImWchar c) {
  const int n = ImStrlenW(InputCharacters);
  if (n + 1 < IM_ARRAYSIZE(InputCharacters)) {
    InputCharacters[n] = c;
    InputCharacters[n + 1] = '\0';
  }
}

void ImGuiIO::AddInputCharactersUTF8(const char *utf8_chars) {
  // We can't pass more wchars than ImGuiIO::InputCharacters[] can hold so don't
  // convert more
  const int wchars_buf_len = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar);
  ImWchar wchars[wchars_buf_len];
  ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL);
  for (int i = 0; i < wchars_buf_len && wchars[i] != 0; i++)
    AddInputCharacter(wchars[i]);
>>>>>>> master
}

//-----------------------------------------------------------------------------
// HELPERS
//-----------------------------------------------------------------------------

<<<<<<< HEAD
#define IM_F32_TO_INT8_UNBOUND(_VAL) \
    ((int)((_VAL)*255.0f + ((_VAL) >= 0 ? 0.5f : -0.5f)))                   // Unsaturated, for display purpose
#define IM_F32_TO_INT8_SAT(_VAL) ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))  // Saturated, always output 0..255
=======
#define IM_F32_TO_INT8_UNBOUND(_VAL)                                           \
  ((int)((_VAL)*255.0f +                                                       \
         ((_VAL) >= 0 ? 0.5f : -0.5f))) // Unsaturated, for display purpose
#define IM_F32_TO_INT8_SAT(_VAL)                                               \
  ((int)(ImSaturate(_VAL) * 255.0f + 0.5f)) // Saturated, always output 0..255
>>>>>>> master

// Play it nice with Windows users. Notepad in 2015 still doesn't display text
// data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

bool ImIsPointInTriangle(const ImVec2 &p, const ImVec2 &a, const ImVec2 &b,
                         const ImVec2 &c) {
  bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
  bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
  bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;
  return ((b1 == b2) && (b2 == b3));
}

<<<<<<< HEAD
int ImStricmp(const char* str1, const char* str2)
{
    int d;
    while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
        str1++;
        str2++;
    }
    return d;
}

int ImStrnicmp(const char* str1, const char* str2, int count)
{
    int d = 0;
    while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
        str1++;
        str2++;
        count--;
    }
    return d;
}

void ImStrncpy(char* dst, const char* src, int count)
{
    if (count < 1) return;
    strncpy(dst, src, (size_t)count);
    dst[count - 1] = 0;
}

char* ImStrdup(const char* str)
{
    size_t len  = strlen(str) + 1;
    void*  buff = ImGui::MemAlloc(len);
    return (char*)memcpy(buff, (const void*)str, len);
=======
int ImStricmp(const char *str1, const char *str2) {
  int d;
  while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
    str1++;
    str2++;
  }
  return d;
}

int ImStrnicmp(const char *str1, const char *str2, int count) {
  int d = 0;
  while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
    str1++;
    str2++;
    count--;
  }
  return d;
}

void ImStrncpy(char *dst, const char *src, int count) {
  if (count < 1)
    return;
  strncpy(dst, src, (size_t)count);
  dst[count - 1] = 0;
}

char *ImStrdup(const char *str) {
  size_t len = strlen(str) + 1;
  void *buff = ImGui::MemAlloc(len);
  return (char *)memcpy(buff, (const void *)str, len);
>>>>>>> master
}

int ImStrlenW(const ImWchar *str) {
  int n = 0;
  while (*str++)
    n++;
  return n;
}

<<<<<<< HEAD
const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin)  // find beginning-of-line
{
    while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n') buf_mid_line--;
    return buf_mid_line;
}

const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
    if (!needle_end) needle_end = needle + strlen(needle);

    const char un0 = (char)toupper(*needle);
    while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end)) {
        if (toupper(*haystack) == un0) {
            const char* b = needle + 1;
            for (const char *a = haystack + 1; b < needle_end; a++, b++)
                if (toupper(*a) != toupper(*b)) break;
            if (b == needle_end) return haystack;
        }
        haystack++;
=======
const ImWchar *ImStrbolW(const ImWchar *buf_mid_line,
                         const ImWchar *buf_begin) // find beginning-of-line
{
  while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n')
    buf_mid_line--;
  return buf_mid_line;
}

const char *ImStristr(const char *haystack, const char *haystack_end,
                      const char *needle, const char *needle_end) {
  if (!needle_end)
    needle_end = needle + strlen(needle);

  const char un0 = (char)toupper(*needle);
  while ((!haystack_end && *haystack) ||
         (haystack_end && haystack < haystack_end)) {
    if (toupper(*haystack) == un0) {
      const char *b = needle + 1;
      for (const char *a = haystack + 1; b < needle_end; a++, b++)
        if (toupper(*a) != toupper(*b))
          break;
      if (b == needle_end)
        return haystack;
>>>>>>> master
    }
    haystack++;
  }
  return NULL;
}

// MSVC version appears to return -1 on overflow, whereas glibc appears to
// return total count (which may be >= buf_size).
// Ideally we would test for only one of those limits at runtime depending on
// the behavior the vsnprintf(), but trying to deduct it at compile time sounds
// like a pandora can of worm.
<<<<<<< HEAD
int ImFormatString(char* buf, int buf_size, const char* fmt, ...)
{
    IM_ASSERT(buf_size > 0);
    va_list args;
    va_start(args, fmt);
    int w = vsnprintf(buf, buf_size, fmt, args);
    va_end(args);
    if (w == -1 || w >= buf_size) w = buf_size - 1;
    buf[w]                          = 0;
    return w;
}

int ImFormatStringV(char* buf, int buf_size, const char* fmt, va_list args)
{
    IM_ASSERT(buf_size > 0);
    int w                           = vsnprintf(buf, buf_size, fmt, args);
    if (w == -1 || w >= buf_size) w = buf_size - 1;
    buf[w]                          = 0;
    return w;
=======
int ImFormatString(char *buf, int buf_size, const char *fmt, ...) {
  IM_ASSERT(buf_size > 0);
  va_list args;
  va_start(args, fmt);
  int w = vsnprintf(buf, buf_size, fmt, args);
  va_end(args);
  if (w == -1 || w >= buf_size)
    w = buf_size - 1;
  buf[w] = 0;
  return w;
}

int ImFormatStringV(char *buf, int buf_size, const char *fmt, va_list args) {
  IM_ASSERT(buf_size > 0);
  int w = vsnprintf(buf, buf_size, fmt, args);
  if (w == -1 || w >= buf_size)
    w = buf_size - 1;
  buf[w] = 0;
  return w;
>>>>>>> master
}

// Pass data_size==0 for zero-terminated strings
// FIXME-OPT: Replace with e.g. FNV1a hash? CRC32 pretty much randomly access
// 1KB. Need to do proper measurements.
<<<<<<< HEAD
ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
    static ImU32 crc32_lut[256] = {0};
    if (!crc32_lut[1]) {
        const ImU32 polynomial = 0xEDB88320;
        for (ImU32 i = 0; i < 256; i++) {
            ImU32 crc = i;
            for (ImU32 j = 0; j < 8; j++) crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
            crc32_lut[i]                      = crc;
        }
=======
ImU32 ImHash(const void *data, int data_size, ImU32 seed) {
  static ImU32 crc32_lut[256] = {0};
  if (!crc32_lut[1]) {
    const ImU32 polynomial = 0xEDB88320;
    for (ImU32 i = 0; i < 256; i++) {
      ImU32 crc = i;
      for (ImU32 j = 0; j < 8; j++)
        crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
      crc32_lut[i] = crc;
>>>>>>> master
    }
  }

<<<<<<< HEAD
    seed                         = ~seed;
    ImU32                crc     = seed;
    const unsigned char* current = (const unsigned char*)data;

    if (data_size > 0) {
        // Known size
        while (data_size--) crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *current++];
    }
    else
    {
        // Zero-terminated string
        while (unsigned char c = *current++) {
            // We support a syntax of "label###id" where only "###id" is included in
            // the hash, and only "label" gets displayed.
            // Because this syntax is rarely used we are optimizing for the common
            // case.
            // - If we reach ### in the string we discard the hash so far and reset to
            // the seed.
            // - We don't do 'current += 2; continue;' after handling ### to keep the
            // code smaller.
            if (c == '#' && current[0] == '#' && current[1] == '#') crc = seed;
            crc                                                         = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
        }
    }
    return ~crc;
=======
  seed = ~seed;
  ImU32 crc = seed;
  const unsigned char *current = (const unsigned char *)data;

  if (data_size > 0) {
    // Known size
    while (data_size--)
      crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *current++];
  } else {
    // Zero-terminated string
    while (unsigned char c = *current++) {
      // We support a syntax of "label###id" where only "###id" is included in
      // the hash, and only "label" gets displayed.
      // Because this syntax is rarely used we are optimizing for the common
      // case.
      // - If we reach ### in the string we discard the hash so far and reset to
      // the seed.
      // - We don't do 'current += 2; continue;' after handling ### to keep the
      // code smaller.
      if (c == '#' && current[0] == '#' && current[1] == '#')
        crc = seed;
      crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
    }
  }
  return ~crc;
>>>>>>> master
}

//-----------------------------------------------------------------------------
// ImText* helpers
//-----------------------------------------------------------------------------

// Convert UTF-8 to 32-bits character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
<<<<<<< HEAD
int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
    unsigned int         c   = (unsigned int)-1;
    const unsigned char* str = (const unsigned char*)in_text;
    if (!(*str & 0x80)) {
        c         = (unsigned int)(*str++);
        *out_char = c;
        return 1;
    }
    if ((*str & 0xe0) == 0xc0) {
        *out_char = 0xFFFD;  // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 2) return 1;
        if (*str < 0xc2) return 2;
        c = (unsigned int)((*str++ & 0x1f) << 6);
        if ((*str & 0xc0) != 0x80) return 2;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 2;
    }
    if ((*str & 0xf0) == 0xe0) {
        *out_char = 0xFFFD;  // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 3) return 1;
        if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
        if (*str == 0xed && str[1] > 0x9f) return 3;  // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x0f) << 12);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 3;
    }
    if ((*str & 0xf8) == 0xf0) {
        *out_char = 0xFFFD;  // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 4) return 1;
        if (*str > 0xf4) return 4;
        if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
        if (*str == 0xf4 && str[1] > 0x8f) return 4;  // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x07) << 18);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 12);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (*str++ & 0x3f);
        // utf-8 encodings of values used in surrogate pairs are invalid
        if ((c & 0xFFFFF800) == 0xD800) return 4;
        *out_char = c;
        return 4;
    }
    *out_char = 0;
    return 0;
}

int ImTextStrFromUtf8(
    ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
    ImWchar* buf_out = buf;
    ImWchar* buf_end = buf + buf_size;
    while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0) break;
        if (c < 0x10000)  // FIXME: Losing characters that don't fit in 2 bytes
            *buf_out++ = (ImWchar)c;
    }
    *buf_out                                  = 0;
    if (in_text_remaining) *in_text_remaining = in_text;
    return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
    int char_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text) {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0) break;
        if (c < 0x10000) char_count++;
    }
    return char_count;
}

// Based on stb_to_utf8() from github.com/nothings/stb/
static inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
    if (c < 0x80) {
        buf[0] = (char)c;
        return 1;
    }
    if (c < 0x800) {
        if (buf_size < 2) return 0;
        buf[0] = (char)(0xc0 + (c >> 6));
        buf[1] = (char)(0x80 + (c & 0x3f));
        return 2;
    }
    if (c >= 0xdc00 && c < 0xe000) {
        return 0;
    }
    if (c >= 0xd800 && c < 0xdc00) {
        if (buf_size < 4) return 0;
        buf[0] = (char)(0xf0 + (c >> 18));
        buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
        buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[3] = (char)(0x80 + ((c)&0x3f));
        return 4;
    }
    // else if (c < 0x10000)
    {
        if (buf_size < 3) return 0;
        buf[0] = (char)(0xe0 + (c >> 12));
        buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[2] = (char)(0x80 + ((c)&0x3f));
        return 3;
    }
}

static inline int ImTextCountUtf8BytesFromChar(unsigned int c)
{
    if (c < 0x80) return 1;
    if (c < 0x800) return 2;
    if (c >= 0xdc00 && c < 0xe000) return 0;
    if (c >= 0xd800 && c < 0xdc00) return 4;
=======
int ImTextCharFromUtf8(unsigned int *out_char, const char *in_text,
                       const char *in_text_end) {
  unsigned int c = (unsigned int)-1;
  const unsigned char *str = (const unsigned char *)in_text;
  if (!(*str & 0x80)) {
    c = (unsigned int)(*str++);
    *out_char = c;
    return 1;
  }
  if ((*str & 0xe0) == 0xc0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char *)str < 2)
      return 1;
    if (*str < 0xc2)
      return 2;
    c = (unsigned int)((*str++ & 0x1f) << 6);
    if ((*str & 0xc0) != 0x80)
      return 2;
    c += (*str++ & 0x3f);
    *out_char = c;
    return 2;
  }
  if ((*str & 0xf0) == 0xe0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char *)str < 3)
      return 1;
    if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf))
      return 3;
    if (*str == 0xed && str[1] > 0x9f)
      return 3; // str[1] < 0x80 is checked below
    c = (unsigned int)((*str++ & 0x0f) << 12);
    if ((*str & 0xc0) != 0x80)
      return 3;
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80)
      return 3;
    c += (*str++ & 0x3f);
    *out_char = c;
    return 3;
  }
  if ((*str & 0xf8) == 0xf0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char *)str < 4)
      return 1;
    if (*str > 0xf4)
      return 4;
    if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf))
      return 4;
    if (*str == 0xf4 && str[1] > 0x8f)
      return 4; // str[1] < 0x80 is checked below
    c = (unsigned int)((*str++ & 0x07) << 18);
    if ((*str & 0xc0) != 0x80)
      return 4;
    c += (unsigned int)((*str++ & 0x3f) << 12);
    if ((*str & 0xc0) != 0x80)
      return 4;
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80)
      return 4;
    c += (*str++ & 0x3f);
    // utf-8 encodings of values used in surrogate pairs are invalid
    if ((c & 0xFFFFF800) == 0xD800)
      return 4;
    *out_char = c;
    return 4;
  }
  *out_char = 0;
  return 0;
}

int ImTextStrFromUtf8(ImWchar *buf, int buf_size, const char *in_text,
                      const char *in_text_end, const char **in_text_remaining) {
  ImWchar *buf_out = buf;
  ImWchar *buf_end = buf + buf_size;
  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) &&
         *in_text) {
    unsigned int c;
    in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
    if (c == 0)
      break;
    if (c < 0x10000) // FIXME: Losing characters that don't fit in 2 bytes
      *buf_out++ = (ImWchar)c;
  }
  *buf_out = 0;
  if (in_text_remaining)
    *in_text_remaining = in_text;
  return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char *in_text, const char *in_text_end) {
  int char_count = 0;
  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c;
    in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
    if (c == 0)
      break;
    if (c < 0x10000)
      char_count++;
  }
  return char_count;
}

// Based on stb_to_utf8() from github.com/nothings/stb/
static inline int ImTextCharToUtf8(char *buf, int buf_size, unsigned int c) {
  if (c < 0x80) {
    buf[0] = (char)c;
    return 1;
  }
  if (c < 0x800) {
    if (buf_size < 2)
      return 0;
    buf[0] = (char)(0xc0 + (c >> 6));
    buf[1] = (char)(0x80 + (c & 0x3f));
    return 2;
  }
  if (c >= 0xdc00 && c < 0xe000) {
    return 0;
  }
  if (c >= 0xd800 && c < 0xdc00) {
    if (buf_size < 4)
      return 0;
    buf[0] = (char)(0xf0 + (c >> 18));
    buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
    buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[3] = (char)(0x80 + ((c)&0x3f));
    return 4;
  }
  // else if (c < 0x10000)
  {
    if (buf_size < 3)
      return 0;
    buf[0] = (char)(0xe0 + (c >> 12));
    buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[2] = (char)(0x80 + ((c)&0x3f));
>>>>>>> master
    return 3;
  }
}

<<<<<<< HEAD
int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
    char*       buf_out = buf;
    const char* buf_end = buf + buf_size;
    while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            *buf_out++ = (char)c;
        else
            buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
    }
    *buf_out = 0;
    return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
    int bytes_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text) {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            bytes_count++;
        else
            bytes_count += ImTextCountUtf8BytesFromChar(c);
    }
    return bytes_count;
}

ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in)
{
    float s = 1.0f / 255.0f;
    return ImVec4(((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_A_SHIFT) & 0xFF) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4& in)
{
    ImU32 out;
    out = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
    return out;
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul)
{
    ImVec4 c = GImGui->Style.Colors[idx];
    c.w *= GImGui->Style.Alpha * alpha_mul;
    return ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(const ImVec4& col)
{
    ImVec4 c = col;
    c.w *= GImGui->Style.Alpha;
    return ColorConvertFloat4ToU32(c);
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]),
// from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b) {
        const float tmp = g;
        g               = b;
        b               = tmp;
        K               = -1.f;
    }
    if (r < g) {
        const float tmp = r;
        r               = g;
        g               = tmp;
        K               = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h              = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s              = chroma / (r + 1e-20f);
    out_v              = r;
=======
static inline int ImTextCountUtf8BytesFromChar(unsigned int c) {
  if (c < 0x80)
    return 1;
  if (c < 0x800)
    return 2;
  if (c >= 0xdc00 && c < 0xe000)
    return 0;
  if (c >= 0xd800 && c < 0xdc00)
    return 4;
  return 3;
}

int ImTextStrToUtf8(char *buf, int buf_size, const ImWchar *in_text,
                    const ImWchar *in_text_end) {
  char *buf_out = buf;
  const char *buf_end = buf + buf_size;
  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) &&
         *in_text) {
    unsigned int c = (unsigned int)(*in_text++);
    if (c < 0x80)
      *buf_out++ = (char)c;
    else
      buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
  }
  *buf_out = 0;
  return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar *in_text,
                                const ImWchar *in_text_end) {
  int bytes_count = 0;
  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c = (unsigned int)(*in_text++);
    if (c < 0x80)
      bytes_count++;
    else
      bytes_count += ImTextCountUtf8BytesFromChar(c);
  }
  return bytes_count;
}

ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in) {
  float s = 1.0f / 255.0f;
  return ImVec4(((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
                ((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
                ((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
                ((in >> IM_COL32_A_SHIFT) & 0xFF) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4 &in) {
  ImU32 out;
  out = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
  out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
  return out;
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul) {
  ImVec4 c = GImGui->Style.Colors[idx];
  c.w *= GImGui->Style.Alpha * alpha_mul;
  return ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(const ImVec4 &col) {
  ImVec4 c = col;
  c.w *= GImGui->Style.Alpha;
  return ColorConvertFloat4ToU32(c);
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]),
// from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float &out_h,
                                 float &out_s, float &out_v) {
  float K = 0.f;
  if (g < b) {
    const float tmp = g;
    g = b;
    b = tmp;
    K = -1.f;
  }
  if (r < g) {
    const float tmp = r;
    r = g;
    g = tmp;
    K = -2.f / 6.f - K;
  }

  const float chroma = r - (g < b ? g : b);
  out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
  out_s = chroma / (r + 1e-20f);
  out_v = r;
>>>>>>> master
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]),
// from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
<<<<<<< HEAD
void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f) {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h       = fmodf(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
        case 0:
            out_r = v;
            out_g = t;
            out_b = p;
            break;
        case 1:
            out_r = q;
            out_g = v;
            out_b = p;
            break;
        case 2:
            out_r = p;
            out_g = v;
            out_b = t;
            break;
        case 3:
            out_r = p;
            out_g = q;
            out_b = v;
            break;
        case 4:
            out_r = t;
            out_g = p;
            out_b = v;
            break;
        case 5:
        default:
            out_r = v;
            out_g = p;
            out_b = q;
            break;
    }
}

FILE* ImFileOpen(const char* filename, const char* mode) { return NULL; }
=======
void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float &out_r,
                                 float &out_g, float &out_b) {
  if (s == 0.0f) {
    // gray
    out_r = out_g = out_b = v;
    return;
  }

  h = fmodf(h, 1.0f) / (60.0f / 360.0f);
  int i = (int)h;
  float f = h - (float)i;
  float p = v * (1.0f - s);
  float q = v * (1.0f - s * f);
  float t = v * (1.0f - s * (1.0f - f));

  switch (i) {
  case 0:
    out_r = v;
    out_g = t;
    out_b = p;
    break;
  case 1:
    out_r = q;
    out_g = v;
    out_b = p;
    break;
  case 2:
    out_r = p;
    out_g = v;
    out_b = t;
    break;
  case 3:
    out_r = p;
    out_g = q;
    out_b = v;
    break;
  case 4:
    out_r = t;
    out_g = p;
    out_b = v;
    break;
  case 5:
  default:
    out_r = v;
    out_g = p;
    out_b = q;
    break;
  }
}

FILE *ImFileOpen(const char *filename, const char *mode) { return NULL; }
>>>>>>> master

// Load file content into memory
// Memory allocated with ImGui::MemAlloc(), must be freed by user using
// ImGui::MemFree()
<<<<<<< HEAD
void* ImFileLoadToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
    assert(0);
    return NULL;
=======
void *ImFileLoadToMemory(const char *filename, const char *file_open_mode,
                         int *out_file_size, int padding_bytes) {
  assert(0);
  return NULL;
>>>>>>> master
}

//-----------------------------------------------------------------------------
// ImGuiStorage
//-----------------------------------------------------------------------------

// Helper: Key->value storage
void ImGuiStorage::Clear() { Data.clear(); }

// std::lower_bound but without the bullshit
<<<<<<< HEAD
static ImVector<ImGuiStorage::Pair>::iterator LowerBound(ImVector<ImGuiStorage::Pair>& data, ImGuiID key)
{
    ImVector<ImGuiStorage::Pair>::iterator first = data.begin();
    ImVector<ImGuiStorage::Pair>::iterator last  = data.end();
    int                                    count = (int)(last - first);
    while (count > 0) {
        int                                    count2 = count / 2;
        ImVector<ImGuiStorage::Pair>::iterator mid    = first + count2;
        if (mid->key < key) {
            first = ++mid;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
=======
static ImVector<ImGuiStorage::Pair>::iterator
LowerBound(ImVector<ImGuiStorage::Pair> &data, ImGuiID key) {
  ImVector<ImGuiStorage::Pair>::iterator first = data.begin();
  ImVector<ImGuiStorage::Pair>::iterator last = data.end();
  int count = (int)(last - first);
  while (count > 0) {
    int count2 = count / 2;
    ImVector<ImGuiStorage::Pair>::iterator mid = first + count2;
    if (mid->key < key) {
      first = ++mid;
      count -= count2 + 1;
    } else {
      count = count2;
>>>>>>> master
    }
  }
  return first;
}

<<<<<<< HEAD
int ImGuiStorage::GetInt(ImGuiID key, int default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key) return default_val;
    return it->val_i;
}

bool ImGuiStorage::GetBool(ImGuiID key, bool default_val) const { return GetInt(key, default_val ? 1 : 0) != 0; }

float ImGuiStorage::GetFloat(ImGuiID key, float default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key) return default_val;
    return it->val_f;
}

void* ImGuiStorage::GetVoidPtr(ImGuiID key) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key) return NULL;
    return it->val_p;
=======
int ImGuiStorage::GetInt(ImGuiID key, int default_val) const {
  ImVector<Pair>::iterator it =
      LowerBound(const_cast<ImVector<ImGuiStorage::Pair> &>(Data), key);
  if (it == Data.end() || it->key != key)
    return default_val;
  return it->val_i;
}

bool ImGuiStorage::GetBool(ImGuiID key, bool default_val) const {
  return GetInt(key, default_val ? 1 : 0) != 0;
}

float ImGuiStorage::GetFloat(ImGuiID key, float default_val) const {
  ImVector<Pair>::iterator it =
      LowerBound(const_cast<ImVector<ImGuiStorage::Pair> &>(Data), key);
  if (it == Data.end() || it->key != key)
    return default_val;
  return it->val_f;
}

void *ImGuiStorage::GetVoidPtr(ImGuiID key) const {
  ImVector<Pair>::iterator it =
      LowerBound(const_cast<ImVector<ImGuiStorage::Pair> &>(Data), key);
  if (it == Data.end() || it->key != key)
    return NULL;
  return it->val_p;
>>>>>>> master
}

// References are only valid until a new value is added to the storage. Calling
// a Set***() function or a Get***Ref() function invalidates the pointer.
<<<<<<< HEAD
int* ImGuiStorage::GetIntRef(ImGuiID key, int default_val)
{
    ImVector<Pair>::iterator it                = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) it = Data.insert(it, Pair(key, default_val));
    return &it->val_i;
}

bool* ImGuiStorage::GetBoolRef(ImGuiID key, bool default_val) { return (bool*)GetIntRef(key, default_val ? 1 : 0); }

float* ImGuiStorage::GetFloatRef(ImGuiID key, float default_val)
{
    ImVector<Pair>::iterator it                = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) it = Data.insert(it, Pair(key, default_val));
    return &it->val_f;
}

void** ImGuiStorage::GetVoidPtrRef(ImGuiID key, void* default_val)
{
    ImVector<Pair>::iterator it                = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) it = Data.insert(it, Pair(key, default_val));
    return &it->val_p;
=======
int *ImGuiStorage::GetIntRef(ImGuiID key, int default_val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key)
    it = Data.insert(it, Pair(key, default_val));
  return &it->val_i;
}

bool *ImGuiStorage::GetBoolRef(ImGuiID key, bool default_val) {
  return (bool *)GetIntRef(key, default_val ? 1 : 0);
}

float *ImGuiStorage::GetFloatRef(ImGuiID key, float default_val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key)
    it = Data.insert(it, Pair(key, default_val));
  return &it->val_f;
}

void **ImGuiStorage::GetVoidPtrRef(ImGuiID key, void *default_val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key)
    it = Data.insert(it, Pair(key, default_val));
  return &it->val_p;
>>>>>>> master
}

// FIXME-OPT: Need a way to reuse the result of lower_bound when doing
// GetInt()/SetInt() - not too bad because it only happens on explicit
// interaction (maximum one a frame)
<<<<<<< HEAD
void ImGuiStorage::SetInt(ImGuiID key, int val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_i = val;
}

void ImGuiStorage::SetBool(ImGuiID key, bool val) { SetInt(key, val ? 1 : 0); }

void ImGuiStorage::SetFloat(ImGuiID key, float val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_f = val;
}

void ImGuiStorage::SetVoidPtr(ImGuiID key, void* val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key) {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_p = val;
}

void ImGuiStorage::SetAllInt(int v)
{
    for (int i = 0; i < Data.Size; i++) Data[i].val_i = v;
=======
void ImGuiStorage::SetInt(ImGuiID key, int val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key) {
    Data.insert(it, Pair(key, val));
    return;
  }
  it->val_i = val;
}

void ImGuiStorage::SetBool(ImGuiID key, bool val) { SetInt(key, val ? 1 : 0); }

void ImGuiStorage::SetFloat(ImGuiID key, float val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key) {
    Data.insert(it, Pair(key, val));
    return;
  }
  it->val_f = val;
}

void ImGuiStorage::SetVoidPtr(ImGuiID key, void *val) {
  ImVector<Pair>::iterator it = LowerBound(Data, key);
  if (it == Data.end() || it->key != key) {
    Data.insert(it, Pair(key, val));
    return;
  }
  it->val_p = val;
}

void ImGuiStorage::SetAllInt(int v) {
  for (int i = 0; i < Data.Size; i++)
    Data[i].val_i = v;
>>>>>>> master
}

//-----------------------------------------------------------------------------
// ImGuiTextFilter
//-----------------------------------------------------------------------------

// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
<<<<<<< HEAD
ImGuiTextFilter::ImGuiTextFilter(const char* default_filter)
{
    if (default_filter) {
        ImStrncpy(InputBuf, default_filter, IM_ARRAYSIZE(InputBuf));
        Build();
    }
    else
    {
        InputBuf[0] = 0;
        CountGrep   = 0;
    }
}

bool ImGuiTextFilter::Draw(const char* label, float width)
{
    if (width != 0.0f) ImGui::PushItemWidth(width);
    bool value_changed = ImGui::InputText(label, InputBuf, IM_ARRAYSIZE(InputBuf));
    if (width != 0.0f) ImGui::PopItemWidth();
    if (value_changed) Build();
    return value_changed;
}

void ImGuiTextFilter::TextRange::split(char separator, ImVector<TextRange>& out)
{
    out.resize(0);
    const char* wb = b;
    const char* we = wb;
    while (we < e) {
        if (*we == separator) {
            out.push_back(TextRange(wb, we));
            wb = we + 1;
        }
        we++;
    }
    if (wb != we) out.push_back(TextRange(wb, we));
}

void ImGuiTextFilter::Build()
{
    Filters.resize(0);
    TextRange input_range(InputBuf, InputBuf + strlen(InputBuf));
    input_range.split(',', Filters);

    CountGrep = 0;
    for (int i = 0; i != Filters.Size; i++) {
        Filters[i].trim_blanks();
        if (Filters[i].empty()) continue;
        if (Filters[i].front() != '-') CountGrep += 1;
    }
}

bool ImGuiTextFilter::PassFilter(const char* text, const char* text_end) const
{
    if (Filters.empty()) return true;

    if (text == NULL) text = "";

    for (int i = 0; i != Filters.Size; i++) {
        const TextRange& f = Filters[i];
        if (f.empty()) continue;
        if (f.front() == '-') {
            // Subtract
            if (ImStristr(text, text_end, f.begin() + 1, f.end()) != NULL) return false;
        }
        else
        {
            // Grep
            if (ImStristr(text, text_end, f.begin(), f.end()) != NULL) return true;
        }
=======
ImGuiTextFilter::ImGuiTextFilter(const char *default_filter) {
  if (default_filter) {
    ImStrncpy(InputBuf, default_filter, IM_ARRAYSIZE(InputBuf));
    Build();
  } else {
    InputBuf[0] = 0;
    CountGrep = 0;
  }
}

bool ImGuiTextFilter::Draw(const char *label, float width) {
  if (width != 0.0f)
    ImGui::PushItemWidth(width);
  bool value_changed =
      ImGui::InputText(label, InputBuf, IM_ARRAYSIZE(InputBuf));
  if (width != 0.0f)
    ImGui::PopItemWidth();
  if (value_changed)
    Build();
  return value_changed;
}

void ImGuiTextFilter::TextRange::split(char separator,
                                       ImVector<TextRange> &out) {
  out.resize(0);
  const char *wb = b;
  const char *we = wb;
  while (we < e) {
    if (*we == separator) {
      out.push_back(TextRange(wb, we));
      wb = we + 1;
>>>>>>> master
    }
    we++;
  }
  if (wb != we)
    out.push_back(TextRange(wb, we));
}

<<<<<<< HEAD
    // Implicit * grep
    if (CountGrep == 0) return true;
=======
void ImGuiTextFilter::Build() {
  Filters.resize(0);
  TextRange input_range(InputBuf, InputBuf + strlen(InputBuf));
  input_range.split(',', Filters);

  CountGrep = 0;
  for (int i = 0; i != Filters.Size; i++) {
    Filters[i].trim_blanks();
    if (Filters[i].empty())
      continue;
    if (Filters[i].front() != '-')
      CountGrep += 1;
  }
}

bool ImGuiTextFilter::PassFilter(const char *text, const char *text_end) const {
  if (Filters.empty())
    return true;

  if (text == NULL)
    text = "";

  for (int i = 0; i != Filters.Size; i++) {
    const TextRange &f = Filters[i];
    if (f.empty())
      continue;
    if (f.front() == '-') {
      // Subtract
      if (ImStristr(text, text_end, f.begin() + 1, f.end()) != NULL)
        return false;
    } else {
      // Grep
      if (ImStristr(text, text_end, f.begin(), f.end()) != NULL)
        return true;
    }
  }
>>>>>>> master

  // Implicit * grep
  if (CountGrep == 0)
    return true;

  return false;
}

//-----------------------------------------------------------------------------
// ImGuiTextBuffer
//-----------------------------------------------------------------------------

// On some platform vsnprintf() takes va_list by reference and modifies it.
// va_copy is the 'correct' way to copy a va_list but Visual Studio prior to
// 2013 doesn't have it.
#ifndef va_copy
#define va_copy(dest, src) (dest = src)
#endif

// Helper: Text buffer for logging/accumulating text
void ImGuiTextBuffer::appendv(const char *fmt, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

<<<<<<< HEAD
    int len = vsnprintf(NULL, 0, fmt, args);  // FIXME-OPT: could do a first pass
                                              // write attempt, likely successful
                                              // on first pass.
    if (len <= 0) return;

    const int write_off = Buf.Size;
    const int needed_sz = write_off + len;
    if (write_off + len >= Buf.Capacity) {
        int double_capacity = Buf.Capacity * 2;
        Buf.reserve(needed_sz > double_capacity ? needed_sz : double_capacity);
    }

    Buf.resize(needed_sz);
    ImFormatStringV(&Buf[write_off] - 1, len + 1, fmt, args_copy);
=======
  int len = vsnprintf(NULL, 0, fmt, args); // FIXME-OPT: could do a first pass
                                           // write attempt, likely successful
                                           // on first pass.
  if (len <= 0)
    return;

  const int write_off = Buf.Size;
  const int needed_sz = write_off + len;
  if (write_off + len >= Buf.Capacity) {
    int double_capacity = Buf.Capacity * 2;
    Buf.reserve(needed_sz > double_capacity ? needed_sz : double_capacity);
  }

  Buf.resize(needed_sz);
  ImFormatStringV(&Buf[write_off] - 1, len + 1, fmt, args_copy);
>>>>>>> master
}

void ImGuiTextBuffer::append(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  appendv(fmt, args);
  va_end(args);
}

//-----------------------------------------------------------------------------
// ImGuiSimpleColumns
//-----------------------------------------------------------------------------

<<<<<<< HEAD
ImGuiSimpleColumns::ImGuiSimpleColumns()
{
    Count   = 0;
    Spacing = Width = NextWidth = 0.0f;
    memset(Pos, 0, sizeof(Pos));
=======
ImGuiSimpleColumns::ImGuiSimpleColumns() {
  Count = 0;
  Spacing = Width = NextWidth = 0.0f;
  memset(Pos, 0, sizeof(Pos));
  memset(NextWidths, 0, sizeof(NextWidths));
}

void ImGuiSimpleColumns::Update(int count, float spacing, bool clear) {
  IM_ASSERT(Count <= IM_ARRAYSIZE(Pos));
  Count = count;
  Width = NextWidth = 0.0f;
  Spacing = spacing;
  if (clear)
>>>>>>> master
    memset(NextWidths, 0, sizeof(NextWidths));
  for (int i = 0; i < Count; i++) {
    if (i > 0 && NextWidths[i] > 0.0f)
      Width += Spacing;
    Pos[i] = (float)(int)Width;
    Width += NextWidths[i];
    NextWidths[i] = 0.0f;
  }
}

float ImGuiSimpleColumns::DeclColumns(
    float w0, float w1,
    float w2) // not using va_arg because they promote float to double
{
<<<<<<< HEAD
    IM_ASSERT(Count <= IM_ARRAYSIZE(Pos));
    Count = count;
    Width = NextWidth = 0.0f;
    Spacing           = spacing;
    if (clear) memset(NextWidths, 0, sizeof(NextWidths));
    for (int i = 0; i < Count; i++) {
        if (i > 0 && NextWidths[i] > 0.0f) Width += Spacing;
        Pos[i] = (float)(int)Width;
        Width += NextWidths[i];
        NextWidths[i] = 0.0f;
    }
}

float ImGuiSimpleColumns::DeclColumns(float w0, float w1,
    float w2)  // not using va_arg because they promote float to double
{
    NextWidth     = 0.0f;
    NextWidths[0] = ImMax(NextWidths[0], w0);
    NextWidths[1] = ImMax(NextWidths[1], w1);
    NextWidths[2] = ImMax(NextWidths[2], w2);
    for (int i = 0; i < 3; i++) NextWidth += NextWidths[i] + ((i > 0 && NextWidths[i] > 0.0f) ? Spacing : 0.0f);
    return ImMax(Width, NextWidth);
}

float ImGuiSimpleColumns::CalcExtraSpace(float avail_w) { return ImMax(0.0f, avail_w - Width); }
=======
  NextWidth = 0.0f;
  NextWidths[0] = ImMax(NextWidths[0], w0);
  NextWidths[1] = ImMax(NextWidths[1], w1);
  NextWidths[2] = ImMax(NextWidths[2], w2);
  for (int i = 0; i < 3; i++)
    NextWidth +=
        NextWidths[i] + ((i > 0 && NextWidths[i] > 0.0f) ? Spacing : 0.0f);
  return ImMax(Width, NextWidth);
}

float ImGuiSimpleColumns::CalcExtraSpace(float avail_w) {
  return ImMax(0.0f, avail_w - Width);
}
>>>>>>> master

//-----------------------------------------------------------------------------
// ImGuiListClipper
//-----------------------------------------------------------------------------

<<<<<<< HEAD
static void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height)
{
    // Set cursor position and a few other things so that SetScrollHere() and
    // Columns() can work when seeking cursor.
    // FIXME: It is problematic that we have to do that here, because
    // custom/equivalent end-user code would stumble on the same issue. Consider
    // moving within SetCursorXXX functions?
    ImGui::SetCursorPosY(pos_y);
    ImGuiWindow* window            = ImGui::GetCurrentWindow();
    window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;    // Setting those fields so that
                                                                              // SetScrollHere() can properly
                                                                              // function after the end of our
                                                                              // clipper usage.
    window->DC.PrevLineHeight = (line_height - GImGui->Style.ItemSpacing.y);  // If we end up needing more
                                                                              // accurate data (to e.g. use
                                                                              // SameLine) we may as well
                                                                              // make the clipper have a
                                                                              // fourth step to let user
                                                                              // process and display the
                                                                              // last item in their list.
    if (window->DC.ColumnsCount > 1)
        window->DC.ColumnsCellMinY = window->DC.CursorPos.y;  // Setting this so that cell Y position are set properly
=======
static void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height) {
  // Set cursor position and a few other things so that SetScrollHere() and
  // Columns() can work when seeking cursor.
  // FIXME: It is problematic that we have to do that here, because
  // custom/equivalent end-user code would stumble on the same issue. Consider
  // moving within SetCursorXXX functions?
  ImGui::SetCursorPosY(pos_y);
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  window->DC.CursorPosPrevLine.y =
      window->DC.CursorPos.y - line_height; // Setting those fields so that
                                            // SetScrollHere() can properly
                                            // function after the end of our
                                            // clipper usage.
  window->DC.PrevLineHeight =
      (line_height - GImGui->Style.ItemSpacing.y); // If we end up needing more
                                                   // accurate data (to e.g. use
                                                   // SameLine) we may as well
                                                   // make the clipper have a
                                                   // fourth step to let user
                                                   // process and display the
                                                   // last item in their list.
  if (window->DC.ColumnsCount > 1)
    window->DC.ColumnsCellMinY =
        window->DC.CursorPos
            .y; // Setting this so that cell Y position are set properly
>>>>>>> master
}

// Use case A: Begin() called from constructor with items_height<0, then called
// again from Sync() in StepNo 1
// Use case B: Begin() called from constructor with items_height>0
// FIXME-LEGACY: Ideally we should remove the Begin/End functions but they are
// part of the legacy API we still support. This is why some of the code in
// Step() calling Begin() and reassign some fields, spaghetti style.
<<<<<<< HEAD
void ImGuiListClipper::Begin(int count, float items_height)
{
    StartPosY   = ImGui::GetCursorPosY();
    ItemsHeight = items_height;
    ItemsCount  = count;
    StepNo      = 0;
    DisplayEnd = DisplayStart = -1;
    if (ItemsHeight > 0.0f) {
        ImGui::CalcListClipping(ItemsCount,
            ItemsHeight,
            &DisplayStart,
            &DisplayEnd);  // calculate how many to clip/display
        if (DisplayStart > 0)
            SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight, ItemsHeight);  // advance cursor
        StepNo = 2;
    }
}

void ImGuiListClipper::End()
{
    if (ItemsCount < 0) return;
    // In theory here we should assert that ImGui::GetCursorPosY() == StartPosY +
    // DisplayEnd * ItemsHeight, but it feels saner to just seek at the end and
    // not assert/crash the user.
    if (ItemsCount < INT_MAX)
        SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight, ItemsHeight);  // advance cursor
    ItemsCount = -1;
    StepNo     = 3;
}

bool ImGuiListClipper::Step()
{
    if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems) {
        ItemsCount = -1;
        return false;
    }
    if (StepNo == 0)  // Step 0: the clipper let you process the first element,
                      // regardless of it being visible or not, so we can measure
                      // the element height.
    {
        DisplayStart = 0;
        DisplayEnd   = 1;
        StartPosY    = ImGui::GetCursorPosY();
        StepNo       = 1;
        return true;
    }
    if (StepNo == 1)  // Step 1: the clipper infer height from first element,
                      // calculate the actual range of elements to display, and
                      // position the cursor before the first element.
    {
        if (ItemsCount == 1) {
            ItemsCount = -1;
            return false;
        }
        float items_height = ImGui::GetCursorPosY() - StartPosY;
        IM_ASSERT(items_height > 0.0f);  // If this triggers, it means Item 0 hasn't
                                         // moved the cursor vertically
        Begin(ItemsCount - 1, items_height);
        DisplayStart++;
        DisplayEnd++;
        StepNo = 3;
        return true;
    }
    if (StepNo == 2)  // Step 2: dummy step only required if an explicit
                      // items_height was passed to constructor or Begin() and user
                      // still call Step(). Does nothing and switch to Step 3.
    {
        IM_ASSERT(DisplayStart >= 0 && DisplayEnd >= 0);
        StepNo = 3;
        return true;
    }
    if (StepNo == 3)  // Step 3: the clipper validate that we have reached the
                      // expected Y position (corresponding to element DisplayEnd),
                      // advance the cursor to the end of the list and then returns
                      // 'false' to end the loop.
        End();
=======
void ImGuiListClipper::Begin(int count, float items_height) {
  StartPosY = ImGui::GetCursorPosY();
  ItemsHeight = items_height;
  ItemsCount = count;
  StepNo = 0;
  DisplayEnd = DisplayStart = -1;
  if (ItemsHeight > 0.0f) {
    ImGui::CalcListClipping(ItemsCount, ItemsHeight, &DisplayStart,
                            &DisplayEnd); // calculate how many to clip/display
    if (DisplayStart > 0)
      SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight,
                                         ItemsHeight); // advance cursor
    StepNo = 2;
  }
}

void ImGuiListClipper::End() {
  if (ItemsCount < 0)
    return;
  // In theory here we should assert that ImGui::GetCursorPosY() == StartPosY +
  // DisplayEnd * ItemsHeight, but it feels saner to just seek at the end and
  // not assert/crash the user.
  if (ItemsCount < INT_MAX)
    SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight,
                                       ItemsHeight); // advance cursor
  ItemsCount = -1;
  StepNo = 3;
}

bool ImGuiListClipper::Step() {
  if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems) {
    ItemsCount = -1;
>>>>>>> master
    return false;
  }
  if (StepNo == 0) // Step 0: the clipper let you process the first element,
                   // regardless of it being visible or not, so we can measure
                   // the element height.
  {
    DisplayStart = 0;
    DisplayEnd = 1;
    StartPosY = ImGui::GetCursorPosY();
    StepNo = 1;
    return true;
  }
  if (StepNo == 1) // Step 1: the clipper infer height from first element,
                   // calculate the actual range of elements to display, and
                   // position the cursor before the first element.
  {
    if (ItemsCount == 1) {
      ItemsCount = -1;
      return false;
    }
    float items_height = ImGui::GetCursorPosY() - StartPosY;
    IM_ASSERT(items_height > 0.0f); // If this triggers, it means Item 0 hasn't
                                    // moved the cursor vertically
    Begin(ItemsCount - 1, items_height);
    DisplayStart++;
    DisplayEnd++;
    StepNo = 3;
    return true;
  }
  if (StepNo == 2) // Step 2: dummy step only required if an explicit
                   // items_height was passed to constructor or Begin() and user
                   // still call Step(). Does nothing and switch to Step 3.
  {
    IM_ASSERT(DisplayStart >= 0 && DisplayEnd >= 0);
    StepNo = 3;
    return true;
  }
  if (StepNo == 3) // Step 3: the clipper validate that we have reached the
                   // expected Y position (corresponding to element DisplayEnd),
                   // advance the cursor to the end of the list and then returns
                   // 'false' to end the loop.
    End();
  return false;
}

//-----------------------------------------------------------------------------
// ImGuiWindow
//-----------------------------------------------------------------------------

<<<<<<< HEAD
ImGuiWindow::ImGuiWindow(const char* name)
{
    Name = ImStrdup(name);
    ID   = ImHash(name, 0);
    IDStack.push_back(ID);
    MoveId = GetID("#MOVE");

    Flags             = 0;
    IndexWithinParent = 0;
    PosFloat = Pos = ImVec2(0.0f, 0.0f);
    Size = SizeFull = ImVec2(0.0f, 0.0f);
    SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    WindowPadding                       = ImVec2(0.0f, 0.0f);
    Scroll                              = ImVec2(0.0f, 0.0f);
    ScrollTarget                        = ImVec2(FLT_MAX, FLT_MAX);
    ScrollTargetCenterRatio             = ImVec2(0.5f, 0.5f);
    ScrollbarX = ScrollbarY = false;
    ScrollbarSizes          = ImVec2(0.0f, 0.0f);
    BorderSize              = 0.0f;
    Active = WasActive = false;
    Accessed           = false;
    Collapsed          = false;
    SkipItems          = false;
    BeginCount         = 0;
    PopupId            = 0;
    AutoFitFramesX = AutoFitFramesY = -1;
    AutoFitOnlyGrows                = false;
    AutoPosLastDirection            = -1;
    HiddenFrames                    = 0;
    SetWindowPosAllowFlags = SetWindowSizeAllowFlags = SetWindowCollapsedAllowFlags =
        ImGuiSetCond_Always | ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing;
    SetWindowPosCenterWanted = false;

    LastFrameActive  = -1;
    ItemWidthDefault = 0.0f;
    FontWindowScale  = 1.0f;

    DrawList = (ImDrawList*)ImGui::MemAlloc(sizeof(ImDrawList));
    IM_PLACEMENT_NEW(DrawList) ImDrawList();
    DrawList->_OwnerName = Name;
    RootWindow           = NULL;
    RootNonPopupWindow   = NULL;
    ParentWindow         = NULL;
=======
ImGuiWindow::ImGuiWindow(const char *name) {
  Name = ImStrdup(name);
  ID = ImHash(name, 0);
  IDStack.push_back(ID);
  MoveId = GetID("#MOVE");

  Flags = 0;
  IndexWithinParent = 0;
  PosFloat = Pos = ImVec2(0.0f, 0.0f);
  Size = SizeFull = ImVec2(0.0f, 0.0f);
  SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
  WindowPadding = ImVec2(0.0f, 0.0f);
  Scroll = ImVec2(0.0f, 0.0f);
  ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
  ScrollTargetCenterRatio = ImVec2(0.5f, 0.5f);
  ScrollbarX = ScrollbarY = false;
  ScrollbarSizes = ImVec2(0.0f, 0.0f);
  BorderSize = 0.0f;
  Active = WasActive = false;
  Accessed = false;
  Collapsed = false;
  SkipItems = false;
  BeginCount = 0;
  PopupId = 0;
  AutoFitFramesX = AutoFitFramesY = -1;
  AutoFitOnlyGrows = false;
  AutoPosLastDirection = -1;
  HiddenFrames = 0;
  SetWindowPosAllowFlags = SetWindowSizeAllowFlags =
      SetWindowCollapsedAllowFlags =
          ImGuiSetCond_Always | ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver |
          ImGuiSetCond_Appearing;
  SetWindowPosCenterWanted = false;

  LastFrameActive = -1;
  ItemWidthDefault = 0.0f;
  FontWindowScale = 1.0f;

  DrawList = (ImDrawList *)ImGui::MemAlloc(sizeof(ImDrawList));
  IM_PLACEMENT_NEW(DrawList) ImDrawList();
  DrawList->_OwnerName = Name;
  RootWindow = NULL;
  RootNonPopupWindow = NULL;
  ParentWindow = NULL;
>>>>>>> master

  FocusIdxAllCounter = FocusIdxTabCounter = -1;
  FocusIdxAllRequestCurrent = FocusIdxTabRequestCurrent = INT_MAX;
  FocusIdxAllRequestNext = FocusIdxTabRequestNext = INT_MAX;
}

ImGuiWindow::~ImGuiWindow() {
  DrawList->~ImDrawList();
  ImGui::MemFree(DrawList);
  DrawList = NULL;
  ImGui::MemFree(Name);
  Name = NULL;
}

<<<<<<< HEAD
ImGuiID ImGuiWindow::GetID(const char* str, const char* str_end)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id   = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
    ImGui::KeepAliveID(id);
    return id;
}

ImGuiID ImGuiWindow::GetID(const void* ptr)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id   = ImHash(&ptr, sizeof(void*), seed);
    ImGui::KeepAliveID(id);
    return id;
=======
ImGuiID ImGuiWindow::GetID(const char *str, const char *str_end) {
  ImGuiID seed = IDStack.back();
  ImGuiID id = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
  ImGui::KeepAliveID(id);
  return id;
}

ImGuiID ImGuiWindow::GetID(const void *ptr) {
  ImGuiID seed = IDStack.back();
  ImGuiID id = ImHash(&ptr, sizeof(void *), seed);
  ImGui::KeepAliveID(id);
  return id;
>>>>>>> master
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const char *str, const char *str_end) {
  ImGuiID seed = IDStack.back();
  return ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
}

//-----------------------------------------------------------------------------
// Internal API exposed in imgui_internal.h
//-----------------------------------------------------------------------------

<<<<<<< HEAD
static void SetCurrentWindow(ImGuiWindow* window)
{
    ImGuiContext& g        = *GImGui;
    g.CurrentWindow        = window;
    if (window) g.FontSize = window->CalcFontSize();
=======
static void SetCurrentWindow(ImGuiWindow *window) {
  ImGuiContext &g = *GImGui;
  g.CurrentWindow = window;
  if (window)
    g.FontSize = window->CalcFontSize();
>>>>>>> master
}

ImGuiWindow *ImGui::GetParentWindow() {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(g.CurrentWindowStack.Size >= 2);
  return g.CurrentWindowStack[(unsigned int)g.CurrentWindowStack.Size - 2];
}

<<<<<<< HEAD
void ImGui::SetActiveID(ImGuiID id, ImGuiWindow* window)
{
    ImGuiContext& g           = *GImGui;
    g.ActiveId                = id;
    g.ActiveIdAllowOverlap    = false;
    g.ActiveIdIsJustActivated = true;
    if (id) g.ActiveIdIsAlive = true;
    g.ActiveIdWindow          = window;
}

void ImGui::ClearActiveID() { SetActiveID(0, NULL); }

void ImGui::SetHoveredID(ImGuiID id)
{
    ImGuiContext& g         = *GImGui;
    g.HoveredId             = id;
    g.HoveredIdAllowOverlap = false;
}

void ImGui::KeepAliveID(ImGuiID id)
{
    ImGuiContext& g                         = *GImGui;
    if (g.ActiveId == id) g.ActiveIdIsAlive = true;
}

// Advance cursor given item size for layout.
void ImGui::ItemSize(const ImVec2& size, float text_offset_y)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    // Always align ourselves on pixel boundaries
    ImGuiContext& g                = *GImGui;
    const float   line_height      = ImMax(window->DC.CurrentLineHeight, size.y);
    const float   text_base_offset = ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
    window->DC.CursorPosPrevLine   = ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
    window->DC.CursorPos = ImVec2((float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX),
        (float)(int)(window->DC.CursorPos.y + line_height + g.Style.ItemSpacing.y));
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);

    // window->DrawList->AddCircle(window->DC.CursorMaxPos, 3.0f,
    // IM_COL32(255,0,0,255), 4); // Debug

    window->DC.PrevLineHeight         = line_height;
    window->DC.PrevLineTextBaseOffset = text_base_offset;
    window->DC.CurrentLineHeight = window->DC.CurrentLineTextBaseOffset = 0.0f;
}

void ImGui::ItemSize(const ImRect& bb, float text_offset_y) { ItemSize(bb.GetSize(), text_offset_y); }
=======
void ImGui::SetActiveID(ImGuiID id, ImGuiWindow *window) {
  ImGuiContext &g = *GImGui;
  g.ActiveId = id;
  g.ActiveIdAllowOverlap = false;
  g.ActiveIdIsJustActivated = true;
  if (id)
    g.ActiveIdIsAlive = true;
  g.ActiveIdWindow = window;
}

void ImGui::ClearActiveID() { SetActiveID(0, NULL); }

void ImGui::SetHoveredID(ImGuiID id) {
  ImGuiContext &g = *GImGui;
  g.HoveredId = id;
  g.HoveredIdAllowOverlap = false;
}

void ImGui::KeepAliveID(ImGuiID id) {
  ImGuiContext &g = *GImGui;
  if (g.ActiveId == id)
    g.ActiveIdIsAlive = true;
}

// Advance cursor given item size for layout.
void ImGui::ItemSize(const ImVec2 &size, float text_offset_y) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  // Always align ourselves on pixel boundaries
  ImGuiContext &g = *GImGui;
  const float line_height = ImMax(window->DC.CurrentLineHeight, size.y);
  const float text_base_offset =
      ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
  window->DC.CursorPosPrevLine =
      ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
  window->DC.CursorPos =
      ImVec2((float)(int)(window->Pos.x + window->DC.IndentX +
                          window->DC.ColumnsOffsetX),
             (float)(int)(window->DC.CursorPos.y + line_height +
                          g.Style.ItemSpacing.y));
  window->DC.CursorMaxPos.x =
      ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
  window->DC.CursorMaxPos.y =
      ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);

  // window->DrawList->AddCircle(window->DC.CursorMaxPos, 3.0f,
  // IM_COL32(255,0,0,255), 4); // Debug

  window->DC.PrevLineHeight = line_height;
  window->DC.PrevLineTextBaseOffset = text_base_offset;
  window->DC.CurrentLineHeight = window->DC.CurrentLineTextBaseOffset = 0.0f;
}

void ImGui::ItemSize(const ImRect &bb, float text_offset_y) {
  ItemSize(bb.GetSize(), text_offset_y);
}
>>>>>>> master

// Declare item bounding box for clipping and interaction.
// Note that the size can be different than the one provided to ItemSize().
// Typically, widgets that spread over available surface
// declares their minimum size requirement to ItemSize() and then use a larger
// region for drawing/interaction, which is passed to ItemAdd().
<<<<<<< HEAD
bool ImGui::ItemAdd(const ImRect& bb, const ImGuiID* id)
{
    ImGuiWindow* window                 = GetCurrentWindow();
    window->DC.LastItemId               = id ? *id : 0;
    window->DC.LastItemRect             = bb;
    window->DC.LastItemHoveredAndUsable = window->DC.LastItemHoveredRect = false;
    if (IsClippedEx(bb, id, false)) return false;

    // This is a sensible default, but widgets are free to override it after
    // calling ItemAdd()
    ImGuiContext& g = *GImGui;
    if (IsMouseHoveringRect(bb.Min, bb.Max)) {
        // Matching the behavior of IsHovered() but allow if
        // ActiveId==window->MoveID (we clicked on the window background)
        // So that clicking on items with no active id such as Text() still returns
        // true with IsItemHovered()
        window->DC.LastItemHoveredRect = true;
        if (g.HoveredRootWindow == window->RootWindow)
            if (g.ActiveId == 0 || (id && g.ActiveId == *id) || g.ActiveIdAllowOverlap ||
                (g.ActiveId == window->MoveId))
                if (IsWindowContentHoverable(window)) window->DC.LastItemHoveredAndUsable = true;
    }

    return true;
}

bool ImGui::IsClippedEx(const ImRect& bb, const ImGuiID* id, bool clip_even_when_logged)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindowRead();

    if (!bb.Overlaps(window->ClipRect))
        if (!id || *id != GImGui->ActiveId)
            if (clip_even_when_logged || !g.LogEnabled) return true;
=======
bool ImGui::ItemAdd(const ImRect &bb, const ImGuiID *id) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.LastItemId = id ? *id : 0;
  window->DC.LastItemRect = bb;
  window->DC.LastItemHoveredAndUsable = window->DC.LastItemHoveredRect = false;
  if (IsClippedEx(bb, id, false))
>>>>>>> master
    return false;

  // This is a sensible default, but widgets are free to override it after
  // calling ItemAdd()
  ImGuiContext &g = *GImGui;
  if (IsMouseHoveringRect(bb.Min, bb.Max)) {
    // Matching the behavior of IsHovered() but allow if
    // ActiveId==window->MoveID (we clicked on the window background)
    // So that clicking on items with no active id such as Text() still returns
    // true with IsItemHovered()
    window->DC.LastItemHoveredRect = true;
    if (g.HoveredRootWindow == window->RootWindow)
      if (g.ActiveId == 0 || (id && g.ActiveId == *id) ||
          g.ActiveIdAllowOverlap || (g.ActiveId == window->MoveId))
        if (IsWindowContentHoverable(window))
          window->DC.LastItemHoveredAndUsable = true;
  }

  return true;
}

<<<<<<< HEAD
// NB: This is an internal helper. The user-facing IsItemHovered() is using data
// emitted from ItemAdd(), with a slightly different logic.
bool ImGui::IsHovered(const ImRect& bb, ImGuiID id, bool flatten_childs)
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId == 0 || g.HoveredId == id || g.HoveredIdAllowOverlap) {
        ImGuiWindow* window = GetCurrentWindowRead();
        if (g.HoveredWindow == window || (flatten_childs && g.HoveredRootWindow == window->RootWindow))
            if ((g.ActiveId == 0 || g.ActiveId == id || g.ActiveIdAllowOverlap) && IsMouseHoveringRect(bb.Min, bb.Max))
                if (IsWindowContentHoverable(g.HoveredRootWindow)) return true;
    }
    return false;
}

bool ImGui::FocusableItemRegister(ImGuiWindow* window, bool is_active, bool tab_stop)
{
    ImGuiContext& g = *GImGui;

    const bool allow_keyboard_focus = window->DC.AllowKeyboardFocus;
    window->FocusIdxAllCounter++;
    if (allow_keyboard_focus) window->FocusIdxTabCounter++;

    // Process keyboard input at this point: TAB, Shift-TAB switch focus
    // We can always TAB out of a widget that doesn't allow tabbing in.
    if (tab_stop && window->FocusIdxAllRequestNext == INT_MAX && window->FocusIdxTabRequestNext == INT_MAX &&
        is_active && IsKeyPressedMap(ImGuiKey_Tab))
    {
        // Modulo on index will be applied at the end of frame once we've got the
        // total counter of items.
        window->FocusIdxTabRequestNext =
            window->FocusIdxTabCounter + (g.IO.KeyShift ? (allow_keyboard_focus ? -1 : 0) : +1);
    }

    if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent) return true;

    if (allow_keyboard_focus)
        if (window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent) return true;

    return false;
=======
bool ImGui::IsClippedEx(const ImRect &bb, const ImGuiID *id,
                        bool clip_even_when_logged) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindowRead();

  if (!bb.Overlaps(window->ClipRect))
    if (!id || *id != GImGui->ActiveId)
      if (clip_even_when_logged || !g.LogEnabled)
        return true;
  return false;
>>>>>>> master
}

// NB: This is an internal helper. The user-facing IsItemHovered() is using data
// emitted from ItemAdd(), with a slightly different logic.
bool ImGui::IsHovered(const ImRect &bb, ImGuiID id, bool flatten_childs) {
  ImGuiContext &g = *GImGui;
  if (g.HoveredId == 0 || g.HoveredId == id || g.HoveredIdAllowOverlap) {
    ImGuiWindow *window = GetCurrentWindowRead();
    if (g.HoveredWindow == window ||
        (flatten_childs && g.HoveredRootWindow == window->RootWindow))
      if ((g.ActiveId == 0 || g.ActiveId == id || g.ActiveIdAllowOverlap) &&
          IsMouseHoveringRect(bb.Min, bb.Max))
        if (IsWindowContentHoverable(g.HoveredRootWindow))
          return true;
  }
  return false;
}

<<<<<<< HEAD
ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y)
{
    ImGuiContext& g = *GImGui;
    ImVec2        content_max;
    if (size.x < 0.0f || size.y < 0.0f) content_max = g.CurrentWindow->Pos + GetContentRegionMax();
    if (size.x <= 0.0f)
        size.x = (size.x == 0.0f) ? default_x : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) + size.x;
    if (size.y <= 0.0f)
        size.y = (size.y == 0.0f) ? default_y : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) + size.y;
    return size;
}

float ImGui::CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x)
{
    if (wrap_pos_x < 0.0f) return 0.0f;

    ImGuiWindow* window = GetCurrentWindowRead();
    if (wrap_pos_x == 0.0f)
        wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
    else if (wrap_pos_x > 0.0f)
        wrap_pos_x += window->Pos.x - window->Scroll.x;  // wrap_pos_x is provided is window local space
=======
bool ImGui::FocusableItemRegister(ImGuiWindow *window, bool is_active,
                                  bool tab_stop) {
  ImGuiContext &g = *GImGui;

  const bool allow_keyboard_focus = window->DC.AllowKeyboardFocus;
  window->FocusIdxAllCounter++;
  if (allow_keyboard_focus)
    window->FocusIdxTabCounter++;

  // Process keyboard input at this point: TAB, Shift-TAB switch focus
  // We can always TAB out of a widget that doesn't allow tabbing in.
  if (tab_stop && window->FocusIdxAllRequestNext == INT_MAX &&
      window->FocusIdxTabRequestNext == INT_MAX && is_active &&
      IsKeyPressedMap(ImGuiKey_Tab)) {
    // Modulo on index will be applied at the end of frame once we've got the
    // total counter of items.
    window->FocusIdxTabRequestNext =
        window->FocusIdxTabCounter +
        (g.IO.KeyShift ? (allow_keyboard_focus ? -1 : 0) : +1);
  }

  if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent)
    return true;

  if (allow_keyboard_focus)
    if (window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent)
      return true;

  return false;
}

void ImGui::FocusableItemUnregister(ImGuiWindow *window) {
  window->FocusIdxAllCounter--;
  window->FocusIdxTabCounter--;
}

ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y) {
  ImGuiContext &g = *GImGui;
  ImVec2 content_max;
  if (size.x < 0.0f || size.y < 0.0f)
    content_max = g.CurrentWindow->Pos + GetContentRegionMax();
  if (size.x <= 0.0f)
    size.x =
        (size.x == 0.0f)
            ? default_x
            : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) +
                  size.x;
  if (size.y <= 0.0f)
    size.y =
        (size.y == 0.0f)
            ? default_y
            : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) +
                  size.y;
  return size;
}
>>>>>>> master

float ImGui::CalcWrapWidthForPos(const ImVec2 &pos, float wrap_pos_x) {
  if (wrap_pos_x < 0.0f)
    return 0.0f;

  ImGuiWindow *window = GetCurrentWindowRead();
  if (wrap_pos_x == 0.0f)
    wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
  else if (wrap_pos_x > 0.0f)
    wrap_pos_x +=
        window->Pos.x -
        window->Scroll.x; // wrap_pos_x is provided is window local space

  return ImMax(wrap_pos_x - pos.x, 1.0f);
}

//-----------------------------------------------------------------------------

void *ImGui::MemAlloc(size_t sz) {
  GImGui->IO.MetricsAllocs++;
  return GImGui->IO.MemAllocFn(sz);
}

void ImGui::MemFree(void *ptr) {
  if (ptr)
    GImGui->IO.MetricsAllocs--;
  return GImGui->IO.MemFreeFn(ptr);
}

const char *ImGui::GetClipboardText() {
  return GImGui->IO.GetClipboardTextFn
             ? GImGui->IO.GetClipboardTextFn(GImGui->IO.ClipboardUserData)
             : "";
}

<<<<<<< HEAD
void ImGui::SetClipboardText(const char* text)
{
    if (GImGui->IO.SetClipboardTextFn) GImGui->IO.SetClipboardTextFn(GImGui->IO.ClipboardUserData, text);
}

const char* ImGui::GetVersion() { return IMGUI_VERSION; }
=======
void ImGui::SetClipboardText(const char *text) {
  if (GImGui->IO.SetClipboardTextFn)
    GImGui->IO.SetClipboardTextFn(GImGui->IO.ClipboardUserData, text);
}

const char *ImGui::GetVersion() { return IMGUI_VERSION; }
>>>>>>> master

// Internal state access - if you want to share ImGui state between modules
// (e.g. DLL) or allocate it yourself
// Note that we still point to some static data and members (such as
// GFontAtlas), so the state instance you end up using will point to the static
// data within its module
<<<<<<< HEAD
ImGuiContext* ImGui::GetCurrentContext() { return GImGui; }
=======
ImGuiContext *ImGui::GetCurrentContext() { return GImGui; }
>>>>>>> master

void ImGui::SetCurrentContext(ImGuiContext *ctx) {
#ifdef IMGUI_SET_CURRENT_CONTEXT_FUNC
<<<<<<< HEAD
    IMGUI_SET_CURRENT_CONTEXT_FUNC(ctx);  // For custom thread-based hackery you
                                          // may want to have control over this.
=======
  IMGUI_SET_CURRENT_CONTEXT_FUNC(ctx); // For custom thread-based hackery you
                                       // may want to have control over this.
>>>>>>> master
#else
  GImGui = ctx;
#endif
}

<<<<<<< HEAD
ImGuiContext* ImGui::CreateContext(void* (*malloc_fn)(size_t), void (*free_fn)(void*))
{
    assert(malloc_fn);
    assert(free_fn);
    ImGuiContext* ctx = (ImGuiContext*)malloc_fn(sizeof(ImGuiContext));
    IM_PLACEMENT_NEW(ctx) ImGuiContext();
    ctx->IO.MemAllocFn = malloc_fn;
    ctx->IO.MemFreeFn  = free_fn;
    return ctx;
}

void ImGui::DestroyContext(ImGuiContext* ctx)
{
    void (*free_fn)(void*) = ctx->IO.MemFreeFn;
    ctx->~ImGuiContext();
    free_fn(ctx);
    if (GImGui == ctx) SetCurrentContext(NULL);
}

ImGuiIO& ImGui::GetIO() { return GImGui->IO; }

ImGuiStyle& ImGui::GetStyle() { return GImGui->Style; }

// Same value as passed to your RenderDrawListsFn() function. valid after
// Render() and until the next call to NewFrame()
ImDrawData* ImGui::GetDrawData() { return GImGui->RenderDrawData.Valid ? &GImGui->RenderDrawData : NULL; }

float ImGui::GetTime() { return GImGui->Time; }

int ImGui::GetFrameCount() { return GImGui->FrameCount; }
=======
ImGuiContext *ImGui::CreateContext(void *(*malloc_fn)(size_t),
                                   void (*free_fn)(void *)) {
  assert(malloc_fn);
  assert(free_fn);
  ImGuiContext *ctx = (ImGuiContext *)malloc_fn(sizeof(ImGuiContext));
  IM_PLACEMENT_NEW(ctx) ImGuiContext();
  ctx->IO.MemAllocFn = malloc_fn;
  ctx->IO.MemFreeFn = free_fn;
  return ctx;
}

void ImGui::DestroyContext(ImGuiContext *ctx) {
  void (*free_fn)(void *) = ctx->IO.MemFreeFn;
  ctx->~ImGuiContext();
  free_fn(ctx);
  if (GImGui == ctx)
    SetCurrentContext(NULL);
}

ImGuiIO &ImGui::GetIO() { return GImGui->IO; }

ImGuiStyle &ImGui::GetStyle() { return GImGui->Style; }

// Same value as passed to your RenderDrawListsFn() function. valid after
// Render() and until the next call to NewFrame()
ImDrawData *ImGui::GetDrawData() {
  return GImGui->RenderDrawData.Valid ? &GImGui->RenderDrawData : NULL;
}

float ImGui::GetTime() { return GImGui->Time; }

int ImGui::GetFrameCount() { return GImGui->FrameCount; }

void ImGui::NewFrame() {
  ImGuiContext &g = *GImGui;

  // Check user data
  IM_ASSERT(g.IO.DeltaTime >= 0.0f); // Need a positive DeltaTime (zero is
                                     // tolerated but will cause some timing
                                     // issues)
  IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f);
  IM_ASSERT(g.IO.Fonts->Fonts.Size > 0); // Font Atlas not created. Did you call
                                         // io.Fonts->GetTexDataAsRGBA32 /
                                         // GetTexDataAsAlpha8 ?
  IM_ASSERT(g.IO.Fonts->Fonts[0]->IsLoaded()); // Font Atlas not created. Did
                                               // you call
                                               // io.Fonts->GetTexDataAsRGBA32 /
                                               // GetTexDataAsAlpha8 ?
  IM_ASSERT(g.Style.CurveTessellationTol > 0.0f); // Invalid style setting
>>>>>>> master

  if (!g.Initialized) {
    // Initialize on first frame
    g.LogClipboard =
        (ImGuiTextBuffer *)ImGui::MemAlloc(sizeof(ImGuiTextBuffer));
    IM_PLACEMENT_NEW(g.LogClipboard) ImGuiTextBuffer();

<<<<<<< HEAD
    // Check user data
    IM_ASSERT(g.IO.DeltaTime >= 0.0f);  // Need a positive DeltaTime (zero is
                                        // tolerated but will cause some timing
                                        // issues)
    IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f);
    IM_ASSERT(g.IO.Fonts->Fonts.Size > 0);           // Font Atlas not created. Did you call
                                                     // io.Fonts->GetTexDataAsRGBA32 /
                                                     // GetTexDataAsAlpha8 ?
    IM_ASSERT(g.IO.Fonts->Fonts[0]->IsLoaded());     // Font Atlas not created. Did
                                                     // you call
                                                     // io.Fonts->GetTexDataAsRGBA32 /
                                                     // GetTexDataAsAlpha8 ?
    IM_ASSERT(g.Style.CurveTessellationTol > 0.0f);  // Invalid style setting

    if (!g.Initialized) {
        // Initialize on first frame
        g.LogClipboard = (ImGuiTextBuffer*)ImGui::MemAlloc(sizeof(ImGuiTextBuffer));
        IM_PLACEMENT_NEW(g.LogClipboard) ImGuiTextBuffer();
=======
    IM_ASSERT(g.Settings.empty());
    LoadIniSettingsFromDisk(g.IO.IniFilename);
    g.Initialized = true;
  }

  SetCurrentFont(GetDefaultFont());
  IM_ASSERT(g.Font->IsLoaded());
>>>>>>> master

  g.Time += g.IO.DeltaTime;
  g.FrameCount += 1;
  g.Tooltip[0] = '\0';
  g.OverlayDrawList.Clear();
  g.OverlayDrawList.PushTextureID(g.IO.Fonts->TexID);
  g.OverlayDrawList.PushClipRectFullScreen();

  // Mark rendering data as invalid to prevent user who may have a handle on it
  // to use it
  g.RenderDrawData.Valid = false;
  g.RenderDrawData.CmdLists = NULL;
  g.RenderDrawData.CmdListsCount = g.RenderDrawData.TotalVtxCount =
      g.RenderDrawData.TotalIdxCount = 0;

  // Update inputs state
  if (g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0)
    g.IO.MousePos = ImVec2(-9999.0f, -9999.0f);
  if ((g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0) ||
      (g.IO.MousePosPrev.x < 0 &&
       g.IO.MousePosPrev.y < 0)) // if mouse just appeared or disappeared
                                 // (negative coordinate) we cancel out movement
                                 // in MouseDelta
    g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
  else
    g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
  g.IO.MousePosPrev = g.IO.MousePos;
  for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++) {
    g.IO.MouseClicked[i] =
        g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
    g.IO.MouseReleased[i] =
        !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
    g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
    g.IO.MouseDownDuration[i] =
        g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f
                                 ? 0.0f
                                 : g.IO.MouseDownDuration[i] + g.IO.DeltaTime)
                          : -1.0f;
    g.IO.MouseDoubleClicked[i] = false;
    if (g.IO.MouseClicked[i]) {
      if (g.Time - g.IO.MouseClickedTime[i] < g.IO.MouseDoubleClickTime) {
        if (ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]) <
            g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
          g.IO.MouseDoubleClicked[i] = true;
        g.IO.MouseClickedTime[i] =
            -FLT_MAX; // so the third click isn't turned into a double-click
      } else {
        g.IO.MouseClickedTime[i] = g.Time;
      }
      g.IO.MouseClickedPos[i] = g.IO.MousePos;
      g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
    } else if (g.IO.MouseDown[i]) {
      g.IO.MouseDragMaxDistanceSqr[i] =
          ImMax(g.IO.MouseDragMaxDistanceSqr[i],
                ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]));
    }
  }
  memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration,
         sizeof(g.IO.KeysDownDuration));
  for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
    g.IO.KeysDownDuration[i] =
        g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f
                                ? 0.0f
                                : g.IO.KeysDownDuration[i] + g.IO.DeltaTime)
                         : -1.0f;

  // Calculate frame-rate for the user, as a purely luxurious feature
  g.FramerateSecPerFrameAccum +=
      g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
  g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
  g.FramerateSecPerFrameIdx =
      (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
  g.IO.Framerate = 1.0f / (g.FramerateSecPerFrameAccum /
                           (float)IM_ARRAYSIZE(g.FramerateSecPerFrame));

  // Clear reference to active widget if the widget isn't alive anymore
  g.HoveredIdPreviousFrame = g.HoveredId;
  g.HoveredId = 0;
  g.HoveredIdAllowOverlap = false;
  if (!g.ActiveIdIsAlive && g.ActiveIdPreviousFrame == g.ActiveId &&
      g.ActiveId != 0)
    ClearActiveID();
  g.ActiveIdPreviousFrame = g.ActiveId;
  g.ActiveIdIsAlive = false;
  g.ActiveIdIsJustActivated = false;

<<<<<<< HEAD
    // Mark rendering data as invalid to prevent user who may have a handle on it
    // to use it
    g.RenderDrawData.Valid         = false;
    g.RenderDrawData.CmdLists      = NULL;
    g.RenderDrawData.CmdListsCount = g.RenderDrawData.TotalVtxCount = g.RenderDrawData.TotalIdxCount = 0;

    // Update inputs state
    if (g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0) g.IO.MousePos = ImVec2(-9999.0f, -9999.0f);
    if ((g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0) ||
        (g.IO.MousePosPrev.x < 0 && g.IO.MousePosPrev.y < 0))  // if mouse just appeared or disappeared
                                                               // (negative coordinate) we cancel out movement
                                                               // in MouseDelta
        g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
    else
        g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
    g.IO.MousePosPrev   = g.IO.MousePos;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++) {
        g.IO.MouseClicked[i]          = g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
        g.IO.MouseReleased[i]         = !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
        g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
        g.IO.MouseDownDuration[i] =
            g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f ? 0.0f : g.IO.MouseDownDuration[i] + g.IO.DeltaTime)
                              : -1.0f;
        g.IO.MouseDoubleClicked[i] = false;
        if (g.IO.MouseClicked[i]) {
            if (g.Time - g.IO.MouseClickedTime[i] < g.IO.MouseDoubleClickTime) {
                if (ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]) <
                    g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
                    g.IO.MouseDoubleClicked[i] = true;
                g.IO.MouseClickedTime[i]       = -FLT_MAX;  // so the third click isn't turned into a double-click
            }
            else
            {
                g.IO.MouseClickedTime[i] = g.Time;
            }
            g.IO.MouseClickedPos[i]         = g.IO.MousePos;
            g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
        }
        else if (g.IO.MouseDown[i])
        {
            g.IO.MouseDragMaxDistanceSqr[i] =
                ImMax(g.IO.MouseDragMaxDistanceSqr[i], ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]));
        }
    }
    memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration, sizeof(g.IO.KeysDownDuration));
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
        g.IO.KeysDownDuration[i] =
            g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f ? 0.0f : g.IO.KeysDownDuration[i] + g.IO.DeltaTime)
                             : -1.0f;

    // Calculate frame-rate for the user, as a purely luxurious feature
    g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
    g.IO.Framerate            = 1.0f / (g.FramerateSecPerFrameAccum / (float)IM_ARRAYSIZE(g.FramerateSecPerFrame));

    // Clear reference to active widget if the widget isn't alive anymore
    g.HoveredIdPreviousFrame = g.HoveredId;
    g.HoveredId              = 0;
    g.HoveredIdAllowOverlap  = false;
    if (!g.ActiveIdIsAlive && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0) ClearActiveID();
    g.ActiveIdPreviousFrame   = g.ActiveId;
    g.ActiveIdIsAlive         = false;
    g.ActiveIdIsJustActivated = false;

    // Handle user moving window (at the beginning of the frame to avoid input lag
    // or sheering). Only valid for root windows.
    if (g.MovedWindowMoveId && g.MovedWindowMoveId == g.ActiveId) {
        KeepAliveID(g.MovedWindowMoveId);
        IM_ASSERT(g.MovedWindow && g.MovedWindow->RootWindow);
        IM_ASSERT(g.MovedWindow->RootWindow->MoveId == g.MovedWindowMoveId);
        if (g.IO.MouseDown[0]) {
            if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoMove)) {
                g.MovedWindow->PosFloat += g.IO.MouseDelta;
                if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoSavedSettings) &&
                    (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f))
                    MarkIniSettingsDirty();
            }
            FocusWindow(g.MovedWindow);
        }
        else
        {
            ClearActiveID();
            g.MovedWindow       = NULL;
            g.MovedWindowMoveId = 0;
        }
    }
    else
    {
        g.MovedWindow       = NULL;
        g.MovedWindowMoveId = 0;
    }

    // Delay saving settings so we don't spam disk too much
    if (g.SettingsDirtyTimer > 0.0f) {
        g.SettingsDirtyTimer -= g.IO.DeltaTime;
        if (g.SettingsDirtyTimer <= 0.0f) SaveIniSettingsToDisk(g.IO.IniFilename);
    }

    // Find the window we are hovering. Child windows can extend beyond the limit
    // of their parent so we need to derive HoveredRootWindow from HoveredWindow
    g.HoveredWindow = g.MovedWindow ? g.MovedWindow : FindHoveredWindow(g.IO.MousePos, false);
    if (g.HoveredWindow && (g.HoveredWindow->Flags & ImGuiWindowFlags_ChildWindow))
        g.HoveredRootWindow = g.HoveredWindow->RootWindow;
    else
        g.HoveredRootWindow = g.MovedWindow ? g.MovedWindow->RootWindow : FindHoveredWindow(g.IO.MousePos, true);

    if (ImGuiWindow* modal_window = GetFrontMostModalRootWindow()) {
        g.ModalWindowDarkeningRatio = ImMin(g.ModalWindowDarkeningRatio + g.IO.DeltaTime * 6.0f, 1.0f);
        ImGuiWindow* window         = g.HoveredRootWindow;
        while (window && window != modal_window) window = window->ParentWindow;
        if (!window) g.HoveredRootWindow = g.HoveredWindow = NULL;
    }
    else
    {
        g.ModalWindowDarkeningRatio = 0.0f;
    }

    // Are we using inputs? Tell user so they can capture/discard the inputs away
    // from the rest of their application.
    // When clicking outside of a window we assume the click is owned by the
    // application and won't request capture. We need to track click ownership.
    int  mouse_earliest_button_down = -1;
    bool mouse_any_down             = false;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++) {
        if (g.IO.MouseClicked[i]) g.IO.MouseDownOwned[i] = (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
        mouse_any_down |= g.IO.MouseDown[i];
        if (g.IO.MouseDown[i])
            if (mouse_earliest_button_down == -1 ||
                g.IO.MouseClickedTime[mouse_earliest_button_down] > g.IO.MouseClickedTime[i])
                mouse_earliest_button_down = i;
    }
    bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) || g.IO.MouseDownOwned[mouse_earliest_button_down];
    if (g.CaptureMouseNextFrame != -1)
        g.IO.WantCaptureMouse = (g.CaptureMouseNextFrame != 0);
    else
        g.IO.WantCaptureMouse = (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) ||
                                (g.ActiveId != 0) || (!g.OpenPopupStack.empty());
    g.IO.WantCaptureKeyboard =
        (g.CaptureKeyboardNextFrame != -1) ? (g.CaptureKeyboardNextFrame != 0) : (g.ActiveId != 0);
    g.IO.WantTextInput      = (g.ActiveId != 0 && g.InputTextState.Id == g.ActiveId);
    g.MouseCursor           = ImGuiMouseCursor_Arrow;
    g.CaptureMouseNextFrame = g.CaptureKeyboardNextFrame = -1;
    g.OsImePosRequest                                    = ImVec2(1.0f, 1.0f);  // OS Input Method Editor showing on
                                                                                // top-left of our window by default

    // If mouse was first clicked outside of ImGui bounds we also cancel out
    // hovering.
    if (!mouse_avail_to_imgui) g.HoveredWindow = g.HoveredRootWindow = NULL;

    // Scale & Scrolling
    if (g.HoveredWindow && g.IO.MouseWheel != 0.0f && !g.HoveredWindow->Collapsed) {
        ImGuiWindow* window = g.HoveredWindow;
        if (g.IO.KeyCtrl) {
            if (g.IO.FontAllowUserScaling) {
                // Zoom / Scale window
                float new_font_scale    = ImClamp(window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
                float scale             = new_font_scale / window->FontWindowScale;
                window->FontWindowScale = new_font_scale;

                const ImVec2 offset = window->Size * (1.0f - scale) * (g.IO.MousePos - window->Pos) / window->Size;
                window->Pos += offset;
                window->PosFloat += offset;
                window->Size *= scale;
                window->SizeFull *= scale;
            }
        }
        else if (!(window->Flags & ImGuiWindowFlags_NoScrollWithMouse))
        {
            // Scroll
            const int scroll_lines = (window->Flags & ImGuiWindowFlags_ComboBox) ? 3 : 5;
            SetWindowScrollY(window, window->Scroll.y - g.IO.MouseWheel * window->CalcFontSize() * scroll_lines);
        }
    }

    // Pressing TAB activate widget focus
    // NB: Don't discard FocusedWindow if it isn't active, so that a window that
    // go on/off programatically won't lose its keyboard focus.
    if (g.ActiveId == 0 && g.FocusedWindow != NULL && g.FocusedWindow->Active && IsKeyPressedMap(ImGuiKey_Tab, false))
        g.FocusedWindow->FocusIdxTabRequestNext = 0;

    // Mark all windows as not visible
    for (int i = 0; i != g.Windows.Size; i++) {
        ImGuiWindow* window = g.Windows[i];
        window->WasActive   = window->Active;
        window->Active      = false;
        window->Accessed    = false;
    }

    // Closing the focused window restore focus to the first active root window in
    // descending z-order
    if (g.FocusedWindow && !g.FocusedWindow->WasActive)
        for (int i = g.Windows.Size - 1; i >= 0; i--)
            if (g.Windows[i]->WasActive && !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow)) {
                FocusWindow(g.Windows[i]);
                break;
            }

    // No window should be open at the beginning of the frame.
    // But in order to allow the user to call NewFrame() multiple times without
    // calling Render(), we are doing an explicit clear.
    g.CurrentWindowStack.resize(0);
    g.CurrentPopupStack.resize(0);
    CloseInactivePopups();

    // Create implicit window - we will only render it if the user has added
    // something to it.
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Debug");
}

// NB: behavior of ImGui after Shutdown() is not tested/guaranteed at the
// moment. This function is merely here to free heap allocations.
void ImGui::Shutdown()
{
    ImGuiContext& g = *GImGui;

    // The fonts atlas can be used prior to calling NewFrame(), so we clear it
    // even if g.Initialized is FALSE (which would happen if we never called
    // NewFrame)
    if (g.IO.Fonts)  // Testing for NULL to allow user to NULLify in case of
                     // running Shutdown() on multiple contexts. Bit hacky.
        g.IO.Fonts->Clear();

    // Cleanup of other data are conditional on actually having used ImGui.
    if (!g.Initialized) return;

    SaveIniSettingsToDisk(g.IO.IniFilename);

    for (int i = 0; i < g.Windows.Size; i++) {
        g.Windows[i]->~ImGuiWindow();
        ImGui::MemFree(g.Windows[i]);
    }
    g.Windows.clear();
    g.WindowsSortBuffer.clear();
    g.CurrentWindow = NULL;
    g.CurrentWindowStack.clear();
    g.FocusedWindow     = NULL;
    g.HoveredWindow     = NULL;
    g.HoveredRootWindow = NULL;
    g.ActiveIdWindow    = NULL;
    g.MovedWindow       = NULL;
    for (int i = 0; i < g.Settings.Size; i++) ImGui::MemFree(g.Settings[i].Name);
    g.Settings.clear();
    g.ColorModifiers.clear();
    g.StyleModifiers.clear();
    g.FontStack.clear();
    g.OpenPopupStack.clear();
    g.CurrentPopupStack.clear();
    g.SetNextWindowSizeConstraintCallback         = NULL;
    g.SetNextWindowSizeConstraintCallbackUserData = NULL;
    for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++) g.RenderDrawLists[i].clear();
    g.OverlayDrawList.ClearFreeMemory();
    g.ColorEditModeStorage.Clear();
    if (g.PrivateClipboard) {
        ImGui::MemFree(g.PrivateClipboard);
        g.PrivateClipboard = NULL;
    }
    g.InputTextState.Text.clear();
    g.InputTextState.InitialText.clear();
    g.InputTextState.TempTextBuffer.clear();

    if (g.LogClipboard) {
        g.LogClipboard->~ImGuiTextBuffer();
        ImGui::MemFree(g.LogClipboard);
    }
=======
  // Handle user moving window (at the beginning of the frame to avoid input lag
  // or sheering). Only valid for root windows.
  if (g.MovedWindowMoveId && g.MovedWindowMoveId == g.ActiveId) {
    KeepAliveID(g.MovedWindowMoveId);
    IM_ASSERT(g.MovedWindow && g.MovedWindow->RootWindow);
    IM_ASSERT(g.MovedWindow->RootWindow->MoveId == g.MovedWindowMoveId);
    if (g.IO.MouseDown[0]) {
      if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoMove)) {
        g.MovedWindow->PosFloat += g.IO.MouseDelta;
        if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoSavedSettings) &&
            (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f))
          MarkIniSettingsDirty();
      }
      FocusWindow(g.MovedWindow);
    } else {
      ClearActiveID();
      g.MovedWindow = NULL;
      g.MovedWindowMoveId = 0;
    }
  } else {
    g.MovedWindow = NULL;
    g.MovedWindowMoveId = 0;
  }

  // Delay saving settings so we don't spam disk too much
  if (g.SettingsDirtyTimer > 0.0f) {
    g.SettingsDirtyTimer -= g.IO.DeltaTime;
    if (g.SettingsDirtyTimer <= 0.0f)
      SaveIniSettingsToDisk(g.IO.IniFilename);
  }
>>>>>>> master

  // Find the window we are hovering. Child windows can extend beyond the limit
  // of their parent so we need to derive HoveredRootWindow from HoveredWindow
  g.HoveredWindow =
      g.MovedWindow ? g.MovedWindow : FindHoveredWindow(g.IO.MousePos, false);
  if (g.HoveredWindow &&
      (g.HoveredWindow->Flags & ImGuiWindowFlags_ChildWindow))
    g.HoveredRootWindow = g.HoveredWindow->RootWindow;
  else
    g.HoveredRootWindow = g.MovedWindow
                              ? g.MovedWindow->RootWindow
                              : FindHoveredWindow(g.IO.MousePos, true);

  if (ImGuiWindow *modal_window = GetFrontMostModalRootWindow()) {
    g.ModalWindowDarkeningRatio =
        ImMin(g.ModalWindowDarkeningRatio + g.IO.DeltaTime * 6.0f, 1.0f);
    ImGuiWindow *window = g.HoveredRootWindow;
    while (window && window != modal_window)
      window = window->ParentWindow;
    if (!window)
      g.HoveredRootWindow = g.HoveredWindow = NULL;
  } else {
    g.ModalWindowDarkeningRatio = 0.0f;
  }

  // Are we using inputs? Tell user so they can capture/discard the inputs away
  // from the rest of their application.
  // When clicking outside of a window we assume the click is owned by the
  // application and won't request capture. We need to track click ownership.
  int mouse_earliest_button_down = -1;
  bool mouse_any_down = false;
  for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++) {
    if (g.IO.MouseClicked[i])
      g.IO.MouseDownOwned[i] =
          (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
    mouse_any_down |= g.IO.MouseDown[i];
    if (g.IO.MouseDown[i])
      if (mouse_earliest_button_down == -1 ||
          g.IO.MouseClickedTime[mouse_earliest_button_down] >
              g.IO.MouseClickedTime[i])
        mouse_earliest_button_down = i;
  }
  bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) ||
                              g.IO.MouseDownOwned[mouse_earliest_button_down];
  if (g.CaptureMouseNextFrame != -1)
    g.IO.WantCaptureMouse = (g.CaptureMouseNextFrame != 0);
  else
    g.IO.WantCaptureMouse =
        (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) ||
        (g.ActiveId != 0) || (!g.OpenPopupStack.empty());
  g.IO.WantCaptureKeyboard = (g.CaptureKeyboardNextFrame != -1)
                                 ? (g.CaptureKeyboardNextFrame != 0)
                                 : (g.ActiveId != 0);
  g.IO.WantTextInput = (g.ActiveId != 0 && g.InputTextState.Id == g.ActiveId);
  g.MouseCursor = ImGuiMouseCursor_Arrow;
  g.CaptureMouseNextFrame = g.CaptureKeyboardNextFrame = -1;
  g.OsImePosRequest = ImVec2(1.0f, 1.0f); // OS Input Method Editor showing on
                                          // top-left of our window by default

  // If mouse was first clicked outside of ImGui bounds we also cancel out
  // hovering.
  if (!mouse_avail_to_imgui)
    g.HoveredWindow = g.HoveredRootWindow = NULL;

  // Scale & Scrolling
  if (g.HoveredWindow && g.IO.MouseWheel != 0.0f &&
      !g.HoveredWindow->Collapsed) {
    ImGuiWindow *window = g.HoveredWindow;
    if (g.IO.KeyCtrl) {
      if (g.IO.FontAllowUserScaling) {
        // Zoom / Scale window
        float new_font_scale = ImClamp(
            window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
        float scale = new_font_scale / window->FontWindowScale;
        window->FontWindowScale = new_font_scale;

        const ImVec2 offset = window->Size * (1.0f - scale) *
                              (g.IO.MousePos - window->Pos) / window->Size;
        window->Pos += offset;
        window->PosFloat += offset;
        window->Size *= scale;
        window->SizeFull *= scale;
      }
    } else if (!(window->Flags & ImGuiWindowFlags_NoScrollWithMouse)) {
      // Scroll
      const int scroll_lines =
          (window->Flags & ImGuiWindowFlags_ComboBox) ? 3 : 5;
      SetWindowScrollY(window, window->Scroll.y -
                                   g.IO.MouseWheel * window->CalcFontSize() *
                                       scroll_lines);
    }
  }

  // Pressing TAB activate widget focus
  // NB: Don't discard FocusedWindow if it isn't active, so that a window that
  // go on/off programatically won't lose its keyboard focus.
  if (g.ActiveId == 0 && g.FocusedWindow != NULL && g.FocusedWindow->Active &&
      IsKeyPressedMap(ImGuiKey_Tab, false))
    g.FocusedWindow->FocusIdxTabRequestNext = 0;

  // Mark all windows as not visible
  for (int i = 0; i != g.Windows.Size; i++) {
    ImGuiWindow *window = g.Windows[i];
    window->WasActive = window->Active;
    window->Active = false;
    window->Accessed = false;
  }

  // Closing the focused window restore focus to the first active root window in
  // descending z-order
  if (g.FocusedWindow && !g.FocusedWindow->WasActive)
    for (int i = g.Windows.Size - 1; i >= 0; i--)
      if (g.Windows[i]->WasActive &&
          !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow)) {
        FocusWindow(g.Windows[i]);
        break;
      }

  // No window should be open at the beginning of the frame.
  // But in order to allow the user to call NewFrame() multiple times without
  // calling Render(), we are doing an explicit clear.
  g.CurrentWindowStack.resize(0);
  g.CurrentPopupStack.resize(0);
  CloseInactivePopups();

  // Create implicit window - we will only render it if the user has added
  // something to it.
  ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Debug");
}

<<<<<<< HEAD
static ImGuiIniData* FindWindowSettings(const char* name)
{
    ImGuiContext& g  = *GImGui;
    ImGuiID       id = ImHash(name, 0);
    for (int i = 0; i != g.Settings.Size; i++) {
        ImGuiIniData* ini = &g.Settings[i];
        if (ini->Id == id) return ini;
    }
    return NULL;
}

static ImGuiIniData* AddWindowSettings(const char* name)
{
    GImGui->Settings.resize(GImGui->Settings.Size + 1);
    ImGuiIniData* ini = &GImGui->Settings.back();
    ini->Name         = ImStrdup(name);
    ini->Id           = ImHash(name, 0);
    ini->Collapsed    = false;
    ini->Pos          = ImVec2(FLT_MAX, FLT_MAX);
    ini->Size         = ImVec2(0, 0);
    return ini;
=======
// NB: behavior of ImGui after Shutdown() is not tested/guaranteed at the
// moment. This function is merely here to free heap allocations.
void ImGui::Shutdown() {
  ImGuiContext &g = *GImGui;

  // The fonts atlas can be used prior to calling NewFrame(), so we clear it
  // even if g.Initialized is FALSE (which would happen if we never called
  // NewFrame)
  if (g.IO.Fonts) // Testing for NULL to allow user to NULLify in case of
                  // running Shutdown() on multiple contexts. Bit hacky.
    g.IO.Fonts->Clear();

  // Cleanup of other data are conditional on actually having used ImGui.
  if (!g.Initialized)
    return;

  SaveIniSettingsToDisk(g.IO.IniFilename);

  for (int i = 0; i < g.Windows.Size; i++) {
    g.Windows[i]->~ImGuiWindow();
    ImGui::MemFree(g.Windows[i]);
  }
  g.Windows.clear();
  g.WindowsSortBuffer.clear();
  g.CurrentWindow = NULL;
  g.CurrentWindowStack.clear();
  g.FocusedWindow = NULL;
  g.HoveredWindow = NULL;
  g.HoveredRootWindow = NULL;
  g.ActiveIdWindow = NULL;
  g.MovedWindow = NULL;
  for (int i = 0; i < g.Settings.Size; i++)
    ImGui::MemFree(g.Settings[i].Name);
  g.Settings.clear();
  g.ColorModifiers.clear();
  g.StyleModifiers.clear();
  g.FontStack.clear();
  g.OpenPopupStack.clear();
  g.CurrentPopupStack.clear();
  g.SetNextWindowSizeConstraintCallback = NULL;
  g.SetNextWindowSizeConstraintCallbackUserData = NULL;
  for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
    g.RenderDrawLists[i].clear();
  g.OverlayDrawList.ClearFreeMemory();
  g.ColorEditModeStorage.Clear();
  if (g.PrivateClipboard) {
    ImGui::MemFree(g.PrivateClipboard);
    g.PrivateClipboard = NULL;
  }
  g.InputTextState.Text.clear();
  g.InputTextState.InitialText.clear();
  g.InputTextState.TempTextBuffer.clear();

  if (g.LogClipboard) {
    g.LogClipboard->~ImGuiTextBuffer();
    ImGui::MemFree(g.LogClipboard);
  }

  g.Initialized = false;
}

static ImGuiIniData *FindWindowSettings(const char *name) {
  ImGuiContext &g = *GImGui;
  ImGuiID id = ImHash(name, 0);
  for (int i = 0; i != g.Settings.Size; i++) {
    ImGuiIniData *ini = &g.Settings[i];
    if (ini->Id == id)
      return ini;
  }
  return NULL;
}

static ImGuiIniData *AddWindowSettings(const char *name) {
  GImGui->Settings.resize(GImGui->Settings.Size + 1);
  ImGuiIniData *ini = &GImGui->Settings.back();
  ini->Name = ImStrdup(name);
  ini->Id = ImHash(name, 0);
  ini->Collapsed = false;
  ini->Pos = ImVec2(FLT_MAX, FLT_MAX);
  ini->Size = ImVec2(0, 0);
  return ini;
>>>>>>> master
}

// Zero-tolerance, poor-man .ini parsing
// FIXME: Write something less rubbish
<<<<<<< HEAD
static void LoadIniSettingsFromDisk(const char* ini_filename) {}

static void SaveIniSettingsToDisk(const char* ini_filename) {}

static void MarkIniSettingsDirty()
{
    ImGuiContext& g                                        = *GImGui;
    if (g.SettingsDirtyTimer <= 0.0f) g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

// FIXME: Add a more explicit sort order in the window structure.
static int ChildWindowComparer(const void* lhs, const void* rhs)
{
    const ImGuiWindow* a = *(const ImGuiWindow**)lhs;
    const ImGuiWindow* b = *(const ImGuiWindow**)rhs;
    if (int d = (a->Flags & ImGuiWindowFlags_Popup) - (b->Flags & ImGuiWindowFlags_Popup)) return d;
    if (int d = (a->Flags & ImGuiWindowFlags_Tooltip) - (b->Flags & ImGuiWindowFlags_Tooltip)) return d;
    if (int d = (a->Flags & ImGuiWindowFlags_ComboBox) - (b->Flags & ImGuiWindowFlags_ComboBox)) return d;
    return (a->IndexWithinParent - b->IndexWithinParent);
}

static void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window)
{
    out_sorted_windows.push_back(window);
    if (window->Active) {
        int count = window->DC.ChildWindows.Size;
        if (count > 1) qsort(window->DC.ChildWindows.begin(), (size_t)count, sizeof(ImGuiWindow*), ChildWindowComparer);
        for (int i = 0; i < count; i++) {
            ImGuiWindow* child = window->DC.ChildWindows[i];
            if (child->Active) AddWindowToSortedBuffer(out_sorted_windows, child);
        }
=======
static void LoadIniSettingsFromDisk(const char *ini_filename) {}

static void SaveIniSettingsToDisk(const char *ini_filename) {}

static void MarkIniSettingsDirty() {
  ImGuiContext &g = *GImGui;
  if (g.SettingsDirtyTimer <= 0.0f)
    g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

// FIXME: Add a more explicit sort order in the window structure.
static int ChildWindowComparer(const void *lhs, const void *rhs) {
  const ImGuiWindow *a = *(const ImGuiWindow **)lhs;
  const ImGuiWindow *b = *(const ImGuiWindow **)rhs;
  if (int d = (a->Flags & ImGuiWindowFlags_Popup) -
              (b->Flags & ImGuiWindowFlags_Popup))
    return d;
  if (int d = (a->Flags & ImGuiWindowFlags_Tooltip) -
              (b->Flags & ImGuiWindowFlags_Tooltip))
    return d;
  if (int d = (a->Flags & ImGuiWindowFlags_ComboBox) -
              (b->Flags & ImGuiWindowFlags_ComboBox))
    return d;
  return (a->IndexWithinParent - b->IndexWithinParent);
}

static void AddWindowToSortedBuffer(ImVector<ImGuiWindow *> &out_sorted_windows,
                                    ImGuiWindow *window) {
  out_sorted_windows.push_back(window);
  if (window->Active) {
    int count = window->DC.ChildWindows.Size;
    if (count > 1)
      qsort(window->DC.ChildWindows.begin(), (size_t)count,
            sizeof(ImGuiWindow *), ChildWindowComparer);
    for (int i = 0; i < count; i++) {
      ImGuiWindow *child = window->DC.ChildWindows[i];
      if (child->Active)
        AddWindowToSortedBuffer(out_sorted_windows, child);
>>>>>>> master
    }
  }
}

<<<<<<< HEAD
static void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list)
{
    if (draw_list->CmdBuffer.empty()) return;

    // Remove trailing command if unused
    ImDrawCmd& last_cmd = draw_list->CmdBuffer.back();
    if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL) {
        draw_list->CmdBuffer.pop_back();
        if (draw_list->CmdBuffer.empty()) return;
    }

    // Draw list sanity check. Detect mismatch between PrimReserve() calls and
    // incrementing _VtxCurrentIdx, _VtxWritePtr etc.
    IM_ASSERT(draw_list->VtxBuffer.Size == 0 ||
              draw_list->_VtxWritePtr == draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
    IM_ASSERT(draw_list->IdxBuffer.Size == 0 ||
              draw_list->_IdxWritePtr == draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
    IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

    // Check that draw_list doesn't use more vertices than indexable (default
    // ImDrawIdx = 2 bytes = 64K vertices)
    // If this assert triggers because you are drawing lots of stuff manually, A)
    // workaround by calling BeginChild()/EndChild() to put your draw commands in
    // multiple draw lists, B) #define ImDrawIdx to a 'unsigned int' in imconfig.h
    // and render accordingly.
    IM_ASSERT((int64_t)draw_list->_VtxCurrentIdx <= ((int64_t)1L << (sizeof(ImDrawIdx) * 8)));  // Too many vertices in
                                                                                                // same ImDrawList. See
                                                                                                // comment above.

    out_render_list.push_back(draw_list);
    GImGui->IO.MetricsRenderVertices += draw_list->VtxBuffer.Size;
    GImGui->IO.MetricsRenderIndices += draw_list->IdxBuffer.Size;
}

static void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window)
{
    AddDrawListToRenderList(out_render_list, window->DrawList);
    for (int i = 0; i < window->DC.ChildWindows.Size; i++) {
        ImGuiWindow* child = window->DC.ChildWindows[i];
        if (!child->Active)  // clipped children may have been marked not active
            continue;
        if ((child->Flags & ImGuiWindowFlags_Popup) && child->HiddenFrames > 0) continue;
        AddWindowToRenderList(out_render_list, child);
    }
=======
static void AddDrawListToRenderList(ImVector<ImDrawList *> &out_render_list,
                                    ImDrawList *draw_list) {
  if (draw_list->CmdBuffer.empty())
    return;

  // Remove trailing command if unused
  ImDrawCmd &last_cmd = draw_list->CmdBuffer.back();
  if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL) {
    draw_list->CmdBuffer.pop_back();
    if (draw_list->CmdBuffer.empty())
      return;
  }

  // Draw list sanity check. Detect mismatch between PrimReserve() calls and
  // incrementing _VtxCurrentIdx, _VtxWritePtr etc.
  IM_ASSERT(draw_list->VtxBuffer.Size == 0 ||
            draw_list->_VtxWritePtr ==
                draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
  IM_ASSERT(draw_list->IdxBuffer.Size == 0 ||
            draw_list->_IdxWritePtr ==
                draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
  IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

  // Check that draw_list doesn't use more vertices than indexable (default
  // ImDrawIdx = 2 bytes = 64K vertices)
  // If this assert triggers because you are drawing lots of stuff manually, A)
  // workaround by calling BeginChild()/EndChild() to put your draw commands in
  // multiple draw lists, B) #define ImDrawIdx to a 'unsigned int' in imconfig.h
  // and render accordingly.
  IM_ASSERT((int64_t)draw_list->_VtxCurrentIdx <=
            ((int64_t)1L << (sizeof(ImDrawIdx) * 8))); // Too many vertices in
                                                       // same ImDrawList. See
                                                       // comment above.

  out_render_list.push_back(draw_list);
  GImGui->IO.MetricsRenderVertices += draw_list->VtxBuffer.Size;
  GImGui->IO.MetricsRenderIndices += draw_list->IdxBuffer.Size;
}

static void AddWindowToRenderList(ImVector<ImDrawList *> &out_render_list,
                                  ImGuiWindow *window) {
  AddDrawListToRenderList(out_render_list, window->DrawList);
  for (int i = 0; i < window->DC.ChildWindows.Size; i++) {
    ImGuiWindow *child = window->DC.ChildWindows[i];
    if (!child->Active) // clipped children may have been marked not active
      continue;
    if ((child->Flags & ImGuiWindowFlags_Popup) && child->HiddenFrames > 0)
      continue;
    AddWindowToRenderList(out_render_list, child);
  }
>>>>>>> master
}

// When using this function it is sane to ensure that float are perfectly
// rounded to integer values, to that e.g. (int)(max.x-min.x) in user's render
// produce correct result.
<<<<<<< HEAD
void ImGui::PushClipRect(
    const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
    window->ClipRect = window->DrawList->_ClipRectStack.back();
=======
void ImGui::PushClipRect(const ImVec2 &clip_rect_min,
                         const ImVec2 &clip_rect_max,
                         bool intersect_with_current_clip_rect) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DrawList->PushClipRect(clip_rect_min, clip_rect_max,
                                 intersect_with_current_clip_rect);
  window->ClipRect = window->DrawList->_ClipRectStack.back();
>>>>>>> master
}

void ImGui::PopClipRect() {
  ImGuiWindow *window = GetCurrentWindow();
  window->DrawList->PopClipRect();
  window->ClipRect = window->DrawList->_ClipRectStack.back();
}

// This is normally called by Render(). You may want to call it directly if you
// want to avoid calling Render() but the gain will be very minimal.
<<<<<<< HEAD
void ImGui::EndFrame()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);                      // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);  // ImGui::EndFrame() called
                                                   // multiple times, or forgot to
                                                   // call ImGui::NewFrame() again

    // Render tooltip
    if (g.Tooltip[0]) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(g.Tooltip);
        ImGui::EndTooltip();
    }

    // Notify OS when our Input Method Editor cursor has moved (e.g. CJK inputs
    // using Microsoft IME)
    if (g.IO.ImeSetInputScreenPosFn && ImLengthSqr(g.OsImePosRequest - g.OsImePosSet) > 0.0001f) {
        g.IO.ImeSetInputScreenPosFn((int)g.OsImePosRequest.x, (int)g.OsImePosRequest.y);
        g.OsImePosSet = g.OsImePosRequest;
    }

    // Hide implicit "Debug" window if it hasn't been used
    IM_ASSERT(g.CurrentWindowStack.Size == 1);  // Mismatched Begin()/End() calls
    if (g.CurrentWindow && !g.CurrentWindow->Accessed) g.CurrentWindow->Active = false;
    ImGui::End();

    // Click to focus window and start moving (after we're done with all our
    // widgets)
    if (g.ActiveId == 0 && g.HoveredId == 0 && g.IO.MouseClicked[0]) {
        if (!(g.FocusedWindow && !g.FocusedWindow->WasActive &&
                g.FocusedWindow->Active))  // Unless we just made a popup appear
        {
            if (g.HoveredRootWindow != NULL) {
                FocusWindow(g.HoveredWindow);
                if (!(g.HoveredWindow->Flags & ImGuiWindowFlags_NoMove)) {
                    g.MovedWindow       = g.HoveredWindow;
                    g.MovedWindowMoveId = g.HoveredRootWindow->MoveId;
                    SetActiveID(g.MovedWindowMoveId, g.HoveredRootWindow);
                }
            }
            else if (g.FocusedWindow != NULL && GetFrontMostModalRootWindow() == NULL)
            {
                // Clicking on void disable focus
                FocusWindow(NULL);
            }
=======
void ImGui::EndFrame() {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(g.Initialized); // Forgot to call ImGui::NewFrame()
  IM_ASSERT(g.FrameCountEnded != g.FrameCount); // ImGui::EndFrame() called
                                                // multiple times, or forgot to
                                                // call ImGui::NewFrame() again

  // Render tooltip
  if (g.Tooltip[0]) {
    ImGui::BeginTooltip();
    ImGui::TextUnformatted(g.Tooltip);
    ImGui::EndTooltip();
  }

  // Notify OS when our Input Method Editor cursor has moved (e.g. CJK inputs
  // using Microsoft IME)
  if (g.IO.ImeSetInputScreenPosFn &&
      ImLengthSqr(g.OsImePosRequest - g.OsImePosSet) > 0.0001f) {
    g.IO.ImeSetInputScreenPosFn((int)g.OsImePosRequest.x,
                                (int)g.OsImePosRequest.y);
    g.OsImePosSet = g.OsImePosRequest;
  }

  // Hide implicit "Debug" window if it hasn't been used
  IM_ASSERT(g.CurrentWindowStack.Size == 1); // Mismatched Begin()/End() calls
  if (g.CurrentWindow && !g.CurrentWindow->Accessed)
    g.CurrentWindow->Active = false;
  ImGui::End();

  // Click to focus window and start moving (after we're done with all our
  // widgets)
  if (g.ActiveId == 0 && g.HoveredId == 0 && g.IO.MouseClicked[0]) {
    if (!(g.FocusedWindow && !g.FocusedWindow->WasActive &&
          g.FocusedWindow->Active)) // Unless we just made a popup appear
    {
      if (g.HoveredRootWindow != NULL) {
        FocusWindow(g.HoveredWindow);
        if (!(g.HoveredWindow->Flags & ImGuiWindowFlags_NoMove)) {
          g.MovedWindow = g.HoveredWindow;
          g.MovedWindowMoveId = g.HoveredRootWindow->MoveId;
          SetActiveID(g.MovedWindowMoveId, g.HoveredRootWindow);
>>>>>>> master
        }
      } else if (g.FocusedWindow != NULL &&
                 GetFrontMostModalRootWindow() == NULL) {
        // Clicking on void disable focus
        FocusWindow(NULL);
      }
    }
  }

<<<<<<< HEAD
    // Sort the window list so that all child windows are after their parent
    // We cannot do that on FocusWindow() because childs may not exist yet
    g.WindowsSortBuffer.resize(0);
    g.WindowsSortBuffer.reserve(g.Windows.Size);
    for (int i = 0; i != g.Windows.Size; i++) {
        ImGuiWindow* window = g.Windows[i];
        if (window->Active && (window->Flags & ImGuiWindowFlags_ChildWindow))  // if a child is active
                                                                               // its parent will add
                                                                               // it
            continue;
        AddWindowToSortedBuffer(g.WindowsSortBuffer, window);
    }
    IM_ASSERT(g.Windows.Size == g.WindowsSortBuffer.Size);  // we done something wrong
    g.Windows.swap(g.WindowsSortBuffer);
=======
  // Sort the window list so that all child windows are after their parent
  // We cannot do that on FocusWindow() because childs may not exist yet
  g.WindowsSortBuffer.resize(0);
  g.WindowsSortBuffer.reserve(g.Windows.Size);
  for (int i = 0; i != g.Windows.Size; i++) {
    ImGuiWindow *window = g.Windows[i];
    if (window->Active &&
        (window->Flags & ImGuiWindowFlags_ChildWindow)) // if a child is active
                                                        // its parent will add
                                                        // it
      continue;
    AddWindowToSortedBuffer(g.WindowsSortBuffer, window);
  }
  IM_ASSERT(g.Windows.Size ==
            g.WindowsSortBuffer.Size); // we done something wrong
  g.Windows.swap(g.WindowsSortBuffer);
>>>>>>> master

  // Clear Input data for next frame
  g.IO.MouseWheel = 0.0f;
  memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));

  g.FrameCountEnded = g.FrameCount;
}

<<<<<<< HEAD
void ImGui::Render()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);  // Forgot to call ImGui::NewFrame()

    if (g.FrameCountEnded != g.FrameCount) ImGui::EndFrame();
    g.FrameCountRendered = g.FrameCount;

    // Skip render altogether if alpha is 0.0
    // Note that vertex buffers have been created and are wasted, so it is best
    // practice that you don't create windows in the first place, or consistently
    // respond to Begin() returning false.
    if (g.Style.Alpha > 0.0f) {
        // Gather windows to render
        g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = g.IO.MetricsActiveWindows = 0;
        for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++) g.RenderDrawLists[i].resize(0);
        for (int i = 0; i != g.Windows.Size; i++) {
            ImGuiWindow* window = g.Windows[i];
            if (window->Active && window->HiddenFrames <= 0 && (window->Flags & (ImGuiWindowFlags_ChildWindow)) == 0) {
                // FIXME: Generalize this with a proper layering system so e.g. user can
                // draw in specific layers, below text, ..
                g.IO.MetricsActiveWindows++;
                if (window->Flags & ImGuiWindowFlags_Popup)
                    AddWindowToRenderList(g.RenderDrawLists[1], window);
                else if (window->Flags & ImGuiWindowFlags_Tooltip)
                    AddWindowToRenderList(g.RenderDrawLists[2], window);
                else
                    AddWindowToRenderList(g.RenderDrawLists[0], window);
            }
        }

        // Flatten layers
        int n              = g.RenderDrawLists[0].Size;
        int flattened_size = n;
        for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++) flattened_size += g.RenderDrawLists[i].Size;
        g.RenderDrawLists[0].resize(flattened_size);
        for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++) {
            ImVector<ImDrawList*>& layer = g.RenderDrawLists[i];
            if (layer.empty()) continue;
            memcpy(&g.RenderDrawLists[0][n], &layer[0], layer.Size * sizeof(ImDrawList*));
            n += layer.Size;
        }

        // Draw software mouse cursor if requested
        if (g.IO.MouseDrawCursor) {
            const ImGuiMouseCursorData& cursor_data = g.MouseCursorData[g.MouseCursor];
            const ImVec2                pos         = g.IO.MousePos - cursor_data.HotOffset;
            const ImVec2                size        = cursor_data.Size;
            const ImTextureID           tex_id      = g.IO.Fonts->TexID;
            g.OverlayDrawList.PushTextureID(tex_id);
            g.OverlayDrawList.AddImage(tex_id,
                pos + ImVec2(1, 0),
                pos + ImVec2(1, 0) + size,
                cursor_data.TexUvMin[1],
                cursor_data.TexUvMax[1],
                IM_COL32(0, 0, 0, 48));  // Shadow
            g.OverlayDrawList.AddImage(tex_id,
                pos + ImVec2(2, 0),
                pos + ImVec2(2, 0) + size,
                cursor_data.TexUvMin[1],
                cursor_data.TexUvMax[1],
                IM_COL32(0, 0, 0, 48));  // Shadow
            g.OverlayDrawList.AddImage(tex_id,
                pos,
                pos + size,
                cursor_data.TexUvMin[1],
                cursor_data.TexUvMax[1],
                IM_COL32(0, 0, 0, 255));  // Black border
            g.OverlayDrawList.AddImage(tex_id,
                pos,
                pos + size,
                cursor_data.TexUvMin[0],
                cursor_data.TexUvMax[0],
                IM_COL32(255, 255, 255, 255));  // White fill
            g.OverlayDrawList.PopTextureID();
        }
        if (!g.OverlayDrawList.VtxBuffer.empty()) AddDrawListToRenderList(g.RenderDrawLists[0], &g.OverlayDrawList);

        // Setup draw data
        g.RenderDrawData.Valid         = true;
        g.RenderDrawData.CmdLists      = (g.RenderDrawLists[0].Size > 0) ? &g.RenderDrawLists[0][0] : NULL;
        g.RenderDrawData.CmdListsCount = g.RenderDrawLists[0].Size;
        g.RenderDrawData.TotalVtxCount = g.IO.MetricsRenderVertices;
        g.RenderDrawData.TotalIdxCount = g.IO.MetricsRenderIndices;

        // Render. If user hasn't set a callback then they may retrieve the draw
        // data via GetDrawData()
        if (g.RenderDrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
            g.IO.RenderDrawListsFn(&g.RenderDrawData);
=======
void ImGui::Render() {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(g.Initialized); // Forgot to call ImGui::NewFrame()

  if (g.FrameCountEnded != g.FrameCount)
    ImGui::EndFrame();
  g.FrameCountRendered = g.FrameCount;

  // Skip render altogether if alpha is 0.0
  // Note that vertex buffers have been created and are wasted, so it is best
  // practice that you don't create windows in the first place, or consistently
  // respond to Begin() returning false.
  if (g.Style.Alpha > 0.0f) {
    // Gather windows to render
    g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices =
        g.IO.MetricsActiveWindows = 0;
    for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
      g.RenderDrawLists[i].resize(0);
    for (int i = 0; i != g.Windows.Size; i++) {
      ImGuiWindow *window = g.Windows[i];
      if (window->Active && window->HiddenFrames <= 0 &&
          (window->Flags & (ImGuiWindowFlags_ChildWindow)) == 0) {
        // FIXME: Generalize this with a proper layering system so e.g. user can
        // draw in specific layers, below text, ..
        g.IO.MetricsActiveWindows++;
        if (window->Flags & ImGuiWindowFlags_Popup)
          AddWindowToRenderList(g.RenderDrawLists[1], window);
        else if (window->Flags & ImGuiWindowFlags_Tooltip)
          AddWindowToRenderList(g.RenderDrawLists[2], window);
        else
          AddWindowToRenderList(g.RenderDrawLists[0], window);
      }
>>>>>>> master
    }

    // Flatten layers
    int n = g.RenderDrawLists[0].Size;
    int flattened_size = n;
    for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
      flattened_size += g.RenderDrawLists[i].Size;
    g.RenderDrawLists[0].resize(flattened_size);
    for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++) {
      ImVector<ImDrawList *> &layer = g.RenderDrawLists[i];
      if (layer.empty())
        continue;
      memcpy(&g.RenderDrawLists[0][n], &layer[0],
             layer.Size * sizeof(ImDrawList *));
      n += layer.Size;
    }

    // Draw software mouse cursor if requested
    if (g.IO.MouseDrawCursor) {
      const ImGuiMouseCursorData &cursor_data =
          g.MouseCursorData[g.MouseCursor];
      const ImVec2 pos = g.IO.MousePos - cursor_data.HotOffset;
      const ImVec2 size = cursor_data.Size;
      const ImTextureID tex_id = g.IO.Fonts->TexID;
      g.OverlayDrawList.PushTextureID(tex_id);
      g.OverlayDrawList.AddImage(
          tex_id, pos + ImVec2(1, 0), pos + ImVec2(1, 0) + size,
          cursor_data.TexUvMin[1], cursor_data.TexUvMax[1],
          IM_COL32(0, 0, 0, 48)); // Shadow
      g.OverlayDrawList.AddImage(
          tex_id, pos + ImVec2(2, 0), pos + ImVec2(2, 0) + size,
          cursor_data.TexUvMin[1], cursor_data.TexUvMax[1],
          IM_COL32(0, 0, 0, 48)); // Shadow
      g.OverlayDrawList.AddImage(
          tex_id, pos, pos + size, cursor_data.TexUvMin[1],
          cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 255)); // Black border
      g.OverlayDrawList.AddImage(
          tex_id, pos, pos + size, cursor_data.TexUvMin[0],
          cursor_data.TexUvMax[0], IM_COL32(255, 255, 255, 255)); // White fill
      g.OverlayDrawList.PopTextureID();
    }
    if (!g.OverlayDrawList.VtxBuffer.empty())
      AddDrawListToRenderList(g.RenderDrawLists[0], &g.OverlayDrawList);

    // Setup draw data
    g.RenderDrawData.Valid = true;
    g.RenderDrawData.CmdLists =
        (g.RenderDrawLists[0].Size > 0) ? &g.RenderDrawLists[0][0] : NULL;
    g.RenderDrawData.CmdListsCount = g.RenderDrawLists[0].Size;
    g.RenderDrawData.TotalVtxCount = g.IO.MetricsRenderVertices;
    g.RenderDrawData.TotalIdxCount = g.IO.MetricsRenderIndices;

    // Render. If user hasn't set a callback then they may retrieve the draw
    // data via GetDrawData()
    if (g.RenderDrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
      g.IO.RenderDrawListsFn(&g.RenderDrawData);
  }
}

<<<<<<< HEAD
const char* ImGui::FindRenderedTextEnd(const char* text, const char* text_end)
{
    const char* text_display_end = text;
    if (!text_end) text_end      = (const char*)-1;

    while (text_display_end < text_end && *text_display_end != '\0' &&
           (text_display_end[0] != '#' || text_display_end[1] != '#'))
        text_display_end++;
    return text_display_end;
}

// Pass text data straight to log (without being displayed)
void ImGui::LogText(const char* fmt, ...) {}
=======
const char *ImGui::FindRenderedTextEnd(const char *text, const char *text_end) {
  const char *text_display_end = text;
  if (!text_end)
    text_end = (const char *)-1;

  while (text_display_end < text_end && *text_display_end != '\0' &&
         (text_display_end[0] != '#' || text_display_end[1] != '#'))
    text_display_end++;
  return text_display_end;
}

// Pass text data straight to log (without being displayed)
void ImGui::LogText(const char *fmt, ...) {}
>>>>>>> master

// Internal version that takes a position to decide on newline placement and pad
// items according to their depth.
// We split text into individual lines to add current tree level padding
<<<<<<< HEAD
static void LogRenderedText(const ImVec2& ref_pos, const char* text, const char* text_end)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = ImGui::GetCurrentWindowRead();

    if (!text_end) text_end = ImGui::FindRenderedTextEnd(text, text_end);

    const bool log_new_line = ref_pos.y > window->DC.LogLinePosY + 1;
    window->DC.LogLinePosY  = ref_pos.y;

    const char* text_remaining = text;
    if (g.LogStartDepth > window->DC.TreeDepth)  // Re-adjust padding if we have
                                                 // popped out of our starting
                                                 // depth
        g.LogStartDepth  = window->DC.TreeDepth;
    const int tree_depth = (window->DC.TreeDepth - g.LogStartDepth);
    for (;;) {
        // Split the string. Each new line (after a '\n') is followed by spacing
        // corresponding to the current depth of our log entry.
        const char* line_end = text_remaining;
        while (line_end < text_end)
            if (*line_end == '\n')
                break;
            else
                line_end++;
        if (line_end >= text_end) line_end = NULL;

        const bool is_first_line = (text == text_remaining);
        bool       is_last_line  = false;
        if (line_end == NULL) {
            is_last_line = true;
            line_end     = text_end;
        }
        if (line_end != NULL && !(is_last_line && (line_end - text_remaining) == 0)) {
            const int char_count = (int)(line_end - text_remaining);
            if (log_new_line || !is_first_line)
                ImGui::LogText(IM_NEWLINE "%*s%.*s", tree_depth * 4, "", char_count, text_remaining);
            else
                ImGui::LogText(" %.*s", char_count, text_remaining);
        }

        if (is_last_line) break;
        text_remaining = line_end + 1;
=======
static void LogRenderedText(const ImVec2 &ref_pos, const char *text,
                            const char *text_end) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = ImGui::GetCurrentWindowRead();

  if (!text_end)
    text_end = ImGui::FindRenderedTextEnd(text, text_end);

  const bool log_new_line = ref_pos.y > window->DC.LogLinePosY + 1;
  window->DC.LogLinePosY = ref_pos.y;

  const char *text_remaining = text;
  if (g.LogStartDepth > window->DC.TreeDepth) // Re-adjust padding if we have
                                              // popped out of our starting
                                              // depth
    g.LogStartDepth = window->DC.TreeDepth;
  const int tree_depth = (window->DC.TreeDepth - g.LogStartDepth);
  for (;;) {
    // Split the string. Each new line (after a '\n') is followed by spacing
    // corresponding to the current depth of our log entry.
    const char *line_end = text_remaining;
    while (line_end < text_end)
      if (*line_end == '\n')
        break;
      else
        line_end++;
    if (line_end >= text_end)
      line_end = NULL;

    const bool is_first_line = (text == text_remaining);
    bool is_last_line = false;
    if (line_end == NULL) {
      is_last_line = true;
      line_end = text_end;
>>>>>>> master
    }
    if (line_end != NULL &&
        !(is_last_line && (line_end - text_remaining) == 0)) {
      const int char_count = (int)(line_end - text_remaining);
      if (log_new_line || !is_first_line)
        ImGui::LogText(IM_NEWLINE "%*s%.*s", tree_depth * 4, "", char_count,
                       text_remaining);
      else
        ImGui::LogText(" %.*s", char_count, text_remaining);
    }

    if (is_last_line)
      break;
    text_remaining = line_end + 1;
  }
}

// Internal ImGui functions to render text
// RenderText***() functions calls ImDrawList::AddText() calls
// ImBitmapFont::RenderText()
<<<<<<< HEAD
void ImGui::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    // Hide anything after a '##' string
    const char* text_display_end;
    if (hide_text_after_hash) {
        text_display_end = FindRenderedTextEnd(text, text_end);
    }
    else
    {
        if (!text_end) text_end = text + strlen(text);  // FIXME-OPT
        text_display_end        = text_end;
    }

    const int text_len = (int)(text_display_end - text);
    if (text_len > 0) {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end);
        if (g.LogEnabled) LogRenderedText(pos, text, text_display_end);
    }
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    if (!text_end) text_end = text + strlen(text);  // FIXME-OPT

    const int text_len = (int)(text_end - text);
    if (text_len > 0) {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_end, wrap_width);
        if (g.LogEnabled) LogRenderedText(pos, text, text_end);
    }
=======
void ImGui::RenderText(ImVec2 pos, const char *text, const char *text_end,
                       bool hide_text_after_hash) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  // Hide anything after a '##' string
  const char *text_display_end;
  if (hide_text_after_hash) {
    text_display_end = FindRenderedTextEnd(text, text_end);
  } else {
    if (!text_end)
      text_end = text + strlen(text); // FIXME-OPT
    text_display_end = text_end;
  }

  const int text_len = (int)(text_display_end - text);
  if (text_len > 0) {
    window->DrawList->AddText(g.Font, g.FontSize, pos,
                              GetColorU32(ImGuiCol_Text), text,
                              text_display_end);
    if (g.LogEnabled)
      LogRenderedText(pos, text, text_display_end);
  }
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char *text,
                              const char *text_end, float wrap_width) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  if (!text_end)
    text_end = text + strlen(text); // FIXME-OPT

  const int text_len = (int)(text_end - text);
  if (text_len > 0) {
    window->DrawList->AddText(g.Font, g.FontSize, pos,
                              GetColorU32(ImGuiCol_Text), text, text_end,
                              wrap_width);
    if (g.LogEnabled)
      LogRenderedText(pos, text, text_end);
  }
>>>>>>> master
}

// Default clip_rect uses (pos_min,pos_max)
// Handle clipping on CPU immediately (vs typically let the GPU clip the
// triangles that are overlapping the clipping rectangle edges)
<<<<<<< HEAD
void ImGui::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end,
    const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    // Hide anything after a '##' string
    const char* text_display_end = FindRenderedTextEnd(text, text_end);
    const int   text_len         = (int)(text_display_end - text);
    if (text_len == 0) return;

    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    // Perform CPU side clipping for single clipped element to avoid using scissor
    // state
    ImVec2       pos = pos_min;
    const ImVec2 text_size =
        text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

    const ImVec2* clip_min      = clip_rect ? &clip_rect->Min : &pos_min;
    const ImVec2* clip_max      = clip_rect ? &clip_rect->Max : &pos_max;
    bool          need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
    if (clip_rect)  // If we had no explicit clipping rectangle then pos==clip_min
        need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

    // Align whole block. We should defer that to the better rendering function
    // when we'll have support for individual line alignment.
    if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
    if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

    // Render
    if (need_clipping) {
        ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
        window->DrawList->AddText(
            g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
    }
    else
    {
        window->DrawList->AddText(
            g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
    }
    if (g.LogEnabled) LogRenderedText(pos, text, text_display_end);
=======
void ImGui::RenderTextClipped(const ImVec2 &pos_min, const ImVec2 &pos_max,
                              const char *text, const char *text_end,
                              const ImVec2 *text_size_if_known,
                              const ImVec2 &align, const ImRect *clip_rect) {
  // Hide anything after a '##' string
  const char *text_display_end = FindRenderedTextEnd(text, text_end);
  const int text_len = (int)(text_display_end - text);
  if (text_len == 0)
    return;

  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  // Perform CPU side clipping for single clipped element to avoid using scissor
  // state
  ImVec2 pos = pos_min;
  const ImVec2 text_size =
      text_size_if_known ? *text_size_if_known
                         : CalcTextSize(text, text_display_end, false, 0.0f);

  const ImVec2 *clip_min = clip_rect ? &clip_rect->Min : &pos_min;
  const ImVec2 *clip_max = clip_rect ? &clip_rect->Max : &pos_max;
  bool need_clipping = (pos.x + text_size.x >= clip_max->x) ||
                       (pos.y + text_size.y >= clip_max->y);
  if (clip_rect) // If we had no explicit clipping rectangle then pos==clip_min
    need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

  // Align whole block. We should defer that to the better rendering function
  // when we'll have support for individual line alignment.
  if (align.x > 0.0f)
    pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
  if (align.y > 0.0f)
    pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

  // Render
  if (need_clipping) {
    ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
    window->DrawList->AddText(g.Font, g.FontSize, pos,
                              GetColorU32(ImGuiCol_Text), text,
                              text_display_end, 0.0f, &fine_clip_rect);
  } else {
    window->DrawList->AddText(g.Font, g.FontSize, pos,
                              GetColorU32(ImGuiCol_Text), text,
                              text_display_end, 0.0f, NULL);
  }
  if (g.LogEnabled)
    LogRenderedText(pos, text, text_display_end);
>>>>>>> master
}

// Render a rectangle shaped with optional rounding and borders
void ImGui::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border,
                        float rounding) {
  ImGuiWindow *window = GetCurrentWindow();

<<<<<<< HEAD
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
    if (border && (window->Flags & ImGuiWindowFlags_ShowBorders)) {
        window->DrawList->AddRect(
            p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding);
    }
}

// Render a triangle to denote expanded/collapsed state
void ImGui::RenderCollapseTriangle(ImVec2 p_min, bool is_open, float scale)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    const float h      = g.FontSize * 1.00f;
    const float r      = h * 0.40f * scale;
    ImVec2      center = p_min + ImVec2(h * 0.50f, h * 0.50f * scale);

    ImVec2 a, b, c;
    if (is_open) {
        center.y -= r * 0.25f;
        a = center + ImVec2(0, 1) * r;
        b = center + ImVec2(-0.866f, -0.5f) * r;
        c = center + ImVec2(0.866f, -0.5f) * r;
    }
    else
    {
        a = center + ImVec2(1, 0) * r;
        b = center + ImVec2(-0.500f, 0.866f) * r;
        c = center + ImVec2(-0.500f, -0.866f) * r;
    }
=======
  window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
  if (border && (window->Flags & ImGuiWindowFlags_ShowBorders)) {
    window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1),
                              GetColorU32(ImGuiCol_BorderShadow), rounding);
    window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border),
                              rounding);
  }
}

// Render a triangle to denote expanded/collapsed state
void ImGui::RenderCollapseTriangle(ImVec2 p_min, bool is_open, float scale) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  const float h = g.FontSize * 1.00f;
  const float r = h * 0.40f * scale;
  ImVec2 center = p_min + ImVec2(h * 0.50f, h * 0.50f * scale);

  ImVec2 a, b, c;
  if (is_open) {
    center.y -= r * 0.25f;
    a = center + ImVec2(0, 1) * r;
    b = center + ImVec2(-0.866f, -0.5f) * r;
    c = center + ImVec2(0.866f, -0.5f) * r;
  } else {
    a = center + ImVec2(1, 0) * r;
    b = center + ImVec2(-0.500f, 0.866f) * r;
    c = center + ImVec2(-0.500f, -0.866f) * r;
  }
>>>>>>> master

  window->DrawList->AddTriangleFilled(a, b, c, GetColorU32(ImGuiCol_Text));
}

<<<<<<< HEAD
void ImGui::RenderBullet(ImVec2 pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->AddCircleFilled(pos, GImGui->FontSize * 0.20f, GetColorU32(ImGuiCol_Text), 8);
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    ImVec2 a, b, c;
    float  start_x   = (float)(int)(g.FontSize * 0.307f + 0.5f);
    float  rem_third = (float)(int)((g.FontSize - start_x) / 3.0f);
    a.x              = pos.x + 0.5f + start_x;
    b.x              = a.x + rem_third;
    c.x              = a.x + rem_third * 3.0f;
    b.y              = pos.y - 1.0f + (float)(int)(g.Font->Ascent * (g.FontSize / g.Font->FontSize) + 0.5f) +
          (float)(int)(g.Font->DisplayOffset.y);
    a.y = b.y - rem_third;
    c.y = b.y - rem_third * 2.0f;
=======
void ImGui::RenderBullet(ImVec2 pos) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DrawList->AddCircleFilled(pos, GImGui->FontSize * 0.20f,
                                    GetColorU32(ImGuiCol_Text), 8);
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  ImVec2 a, b, c;
  float start_x = (float)(int)(g.FontSize * 0.307f + 0.5f);
  float rem_third = (float)(int)((g.FontSize - start_x) / 3.0f);
  a.x = pos.x + 0.5f + start_x;
  b.x = a.x + rem_third;
  c.x = a.x + rem_third * 3.0f;
  b.y = pos.y - 1.0f +
        (float)(int)(g.Font->Ascent * (g.FontSize / g.Font->FontSize) + 0.5f) +
        (float)(int)(g.Font->DisplayOffset.y);
  a.y = b.y - rem_third;
  c.y = b.y - rem_third * 2.0f;
>>>>>>> master

  window->DrawList->PathLineTo(a);
  window->DrawList->PathLineTo(b);
  window->DrawList->PathLineTo(c);
  window->DrawList->PathStroke(col, false);
}

// Calculate text size. Text can be multi-line. Optionally ignore text after a
// ## marker.
// CalcTextSize("") should return ImVec2(0.0f, GImGui->FontSize)
ImVec2 ImGui::CalcTextSize(const char *text, const char *text_end,
                           bool hide_text_after_double_hash, float wrap_width) {
  ImGuiContext &g = *GImGui;

<<<<<<< HEAD
    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = FindRenderedTextEnd(text, text_end);  // Hide anything after a '##' string
    else
        text_display_end = text_end;

    ImFont*     font      = g.Font;
    const float font_size = g.FontSize;
    if (text == text_display_end) return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

    // Cancel out character spacing for the last character of a line (it is baked
    // into glyph->XAdvance field)
    const float font_scale          = font_size / font->FontSize;
    const float character_spacing_x = 1.0f * font_scale;
    if (text_size.x > 0.0f) text_size.x -= character_spacing_x;
    text_size.x = (float)(int)(text_size.x + 0.95f);
=======
  const char *text_display_end;
  if (hide_text_after_double_hash)
    text_display_end = FindRenderedTextEnd(
        text, text_end); // Hide anything after a '##' string
  else
    text_display_end = text_end;

  ImFont *font = g.Font;
  const float font_size = g.FontSize;
  if (text == text_display_end)
    return ImVec2(0.0f, font_size);
  ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text,
                                         text_display_end, NULL);

  // Cancel out character spacing for the last character of a line (it is baked
  // into glyph->XAdvance field)
  const float font_scale = font_size / font->FontSize;
  const float character_spacing_x = 1.0f * font_scale;
  if (text_size.x > 0.0f)
    text_size.x -= character_spacing_x;
  text_size.x = (float)(int)(text_size.x + 0.95f);
>>>>>>> master

  return text_size;
}

// Helper to calculate coarse clipping of large list of evenly sized items.
// NB: Prefer using the ImGuiListClipper higher-level helper if you can! Read
// comments and instructions there on how those use this sort of pattern.
// NB: 'items_count' is only used to clamp the result, if you don't know your
// count you can use INT_MAX
<<<<<<< HEAD
void ImGui::CalcListClipping(
    int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindowRead();
    if (g.LogEnabled) {
        // If logging is active, do not perform any clipping
        *out_items_display_start = 0;
        *out_items_display_end   = items_count;
        return;
    }
    if (window->SkipItems) {
        *out_items_display_start = *out_items_display_end = 0;
        return;
    }

    const ImVec2 pos         = window->DC.CursorPos;
    int          start       = (int)((window->ClipRect.Min.y - pos.y) / items_height);
    int          end         = (int)((window->ClipRect.Max.y - pos.y) / items_height);
    start                    = ImClamp(start, 0, items_count);
    end                      = ImClamp(end + 1, start, items_count);
    *out_items_display_start = start;
    *out_items_display_end   = end;
=======
void ImGui::CalcListClipping(int items_count, float items_height,
                             int *out_items_display_start,
                             int *out_items_display_end) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindowRead();
  if (g.LogEnabled) {
    // If logging is active, do not perform any clipping
    *out_items_display_start = 0;
    *out_items_display_end = items_count;
    return;
  }
  if (window->SkipItems) {
    *out_items_display_start = *out_items_display_end = 0;
    return;
  }

  const ImVec2 pos = window->DC.CursorPos;
  int start = (int)((window->ClipRect.Min.y - pos.y) / items_height);
  int end = (int)((window->ClipRect.Max.y - pos.y) / items_height);
  start = ImClamp(start, 0, items_count);
  end = ImClamp(end + 1, start, items_count);
  *out_items_display_start = start;
  *out_items_display_end = end;
>>>>>>> master
}

// Find window given position, search front-to-back
// FIXME: Note that we have a lag here because WindowRectClipped is updated in
// Begin() so windows moved by user via SetWindowPos() and not
// SetNextWindowPos() will have that rectangle lagging by a frame at the time
// FindHoveredWindow() is called, aka before the next Begin(). Moving window
// thankfully isn't affected.
<<<<<<< HEAD
static ImGuiWindow* FindHoveredWindow(ImVec2 pos, bool excluding_childs)
{
    ImGuiContext& g = *GImGui;
    for (int i = g.Windows.Size - 1; i >= 0; i--) {
        ImGuiWindow* window = g.Windows[i];
        if (!window->Active) continue;
        if (window->Flags & ImGuiWindowFlags_NoInputs) continue;
        if (excluding_childs && (window->Flags & ImGuiWindowFlags_ChildWindow) != 0) continue;

        // Using the clipped AABB so a child window will typically be clipped by its
        // parent.
        ImRect bb(window->WindowRectClipped.Min - g.Style.TouchExtraPadding,
            window->WindowRectClipped.Max + g.Style.TouchExtraPadding);
        if (bb.Contains(pos)) return window;
    }
    return NULL;
=======
static ImGuiWindow *FindHoveredWindow(ImVec2 pos, bool excluding_childs) {
  ImGuiContext &g = *GImGui;
  for (int i = g.Windows.Size - 1; i >= 0; i--) {
    ImGuiWindow *window = g.Windows[i];
    if (!window->Active)
      continue;
    if (window->Flags & ImGuiWindowFlags_NoInputs)
      continue;
    if (excluding_childs && (window->Flags & ImGuiWindowFlags_ChildWindow) != 0)
      continue;

    // Using the clipped AABB so a child window will typically be clipped by its
    // parent.
    ImRect bb(window->WindowRectClipped.Min - g.Style.TouchExtraPadding,
              window->WindowRectClipped.Max + g.Style.TouchExtraPadding);
    if (bb.Contains(pos))
      return window;
  }
  return NULL;
>>>>>>> master
}

// Test if mouse cursor is hovering given rectangle
// NB- Rectangle is clipped by our current clip setting
// NB- Expand the rectangle to be generous on imprecise inputs systems
// (g.Style.TouchExtraPadding)
<<<<<<< HEAD
bool ImGui::IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindowRead();

    // Clip
    ImRect rect_clipped(r_min, r_max);
    if (clip) rect_clipped.Clip(window->ClipRect);

    // Expand for touch input
    const ImRect rect_for_touch(
        rect_clipped.Min - g.Style.TouchExtraPadding, rect_clipped.Max + g.Style.TouchExtraPadding);
    return rect_for_touch.Contains(g.IO.MousePos);
=======
bool ImGui::IsMouseHoveringRect(const ImVec2 &r_min, const ImVec2 &r_max,
                                bool clip) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindowRead();

  // Clip
  ImRect rect_clipped(r_min, r_max);
  if (clip)
    rect_clipped.Clip(window->ClipRect);

  // Expand for touch input
  const ImRect rect_for_touch(rect_clipped.Min - g.Style.TouchExtraPadding,
                              rect_clipped.Max + g.Style.TouchExtraPadding);
  return rect_for_touch.Contains(g.IO.MousePos);
>>>>>>> master
}

bool ImGui::IsMouseHoveringWindow() {
  ImGuiContext &g = *GImGui;
  return g.HoveredWindow == g.CurrentWindow;
}

bool ImGui::IsMouseHoveringAnyWindow() {
  ImGuiContext &g = *GImGui;
  return g.HoveredWindow != NULL;
}

<<<<<<< HEAD
bool ImGui::IsPosHoveringAnyWindow(const ImVec2& pos) { return FindHoveredWindow(pos, false) != NULL; }
=======
bool ImGui::IsPosHoveringAnyWindow(const ImVec2 &pos) {
  return FindHoveredWindow(pos, false) != NULL;
}
>>>>>>> master

static bool IsKeyPressedMap(ImGuiKey key, bool repeat) {
  const int key_index = GImGui->IO.KeyMap[key];
  return ImGui::IsKeyPressed(key_index, repeat);
}

int ImGui::GetKeyIndex(ImGuiKey imgui_key) {
  IM_ASSERT(imgui_key >= 0 && imgui_key < ImGuiKey_COUNT);
  return GImGui->IO.KeyMap[imgui_key];
}

// Note that imgui doesn't know the semantic of each entry of io.KeyDown[]. Use
// your own indices/enums according to how your backend/engine stored them into
// KeyDown[]!
<<<<<<< HEAD
bool ImGui::IsKeyDown(int user_key_index)
{
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(GImGui->IO.KeysDown));
    return GImGui->IO.KeysDown[user_key_index];
}

bool ImGui::IsKeyPressed(int user_key_index, bool repeat)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    const float t = g.IO.KeysDownDuration[user_key_index];
    if (t == 0.0f) return true;

    if (repeat && t > g.IO.KeyRepeatDelay) {
        float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
        if ((fmodf(t - delay, rate) > rate * 0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate * 0.5f))
            return true;
    }
=======
bool ImGui::IsKeyDown(int user_key_index) {
  if (user_key_index < 0)
>>>>>>> master
    return false;
  IM_ASSERT(user_key_index >= 0 &&
            user_key_index < IM_ARRAYSIZE(GImGui->IO.KeysDown));
  return GImGui->IO.KeysDown[user_key_index];
}

<<<<<<< HEAD
bool ImGui::IsKeyReleased(int user_key_index)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    if (g.IO.KeysDownDurationPrev[user_key_index] >= 0.0f && !g.IO.KeysDown[user_key_index]) return true;
=======
bool ImGui::IsKeyPressed(int user_key_index, bool repeat) {
  ImGuiContext &g = *GImGui;
  if (user_key_index < 0)
>>>>>>> master
    return false;
  IM_ASSERT(user_key_index >= 0 &&
            user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
  const float t = g.IO.KeysDownDuration[user_key_index];
  if (t == 0.0f)
    return true;

  if (repeat && t > g.IO.KeyRepeatDelay) {
    float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
    if ((fmodf(t - delay, rate) > rate * 0.5f) !=
        (fmodf(t - delay - g.IO.DeltaTime, rate) > rate * 0.5f))
      return true;
  }
  return false;
}

<<<<<<< HEAD
bool ImGui::IsMouseDown(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDown[button];
}

bool ImGui::IsMouseClicked(int button, bool repeat)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    const float t = g.IO.MouseDownDuration[button];
    if (t == 0.0f) return true;

    if (repeat && t > g.IO.KeyRepeatDelay) {
        float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
        if ((fmodf(t - delay, rate) > rate * 0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate * 0.5f))
            return true;
    }

=======
bool ImGui::IsKeyReleased(int user_key_index) {
  ImGuiContext &g = *GImGui;
  if (user_key_index < 0)
>>>>>>> master
    return false;
  IM_ASSERT(user_key_index >= 0 &&
            user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
  if (g.IO.KeysDownDurationPrev[user_key_index] >= 0.0f &&
      !g.IO.KeysDown[user_key_index])
    return true;
  return false;
}

bool ImGui::IsMouseDown(int button) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  return g.IO.MouseDown[button];
}

bool ImGui::IsMouseClicked(int button, bool repeat) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  const float t = g.IO.MouseDownDuration[button];
  if (t == 0.0f)
    return true;

  if (repeat && t > g.IO.KeyRepeatDelay) {
    float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
    if ((fmodf(t - delay, rate) > rate * 0.5f) !=
        (fmodf(t - delay - g.IO.DeltaTime, rate) > rate * 0.5f))
      return true;
  }

  return false;
}

<<<<<<< HEAD
bool ImGui::IsMouseDragging(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (!g.IO.MouseDown[button]) return false;
    if (lock_threshold < 0.0f) lock_threshold = g.IO.MouseDragThreshold;
    return g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold;
}

ImVec2 ImGui::GetMousePos() { return GImGui->IO.MousePos; }

// NB: prefer to call right after BeginPopup(). At the time Selectable/MenuItem
// is activated, the popup is already closed!
ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup()
{
    ImGuiContext& g = *GImGui;
    if (g.CurrentPopupStack.Size > 0) return g.OpenPopupStack[g.CurrentPopupStack.Size - 1].MousePosOnOpen;
    return g.IO.MousePos;
}

ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (lock_threshold < 0.0f) lock_threshold = g.IO.MouseDragThreshold;
    if (g.IO.MouseDown[button])
        if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
            return g.IO.MousePos - g.IO.MouseClickedPos[button];  // Assume we can only
                                                                  // get active with
                                                                  // left-mouse button
                                                                  // (at the moment).
    return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    // NB: We don't need to reset g.IO.MouseDragMaxDistanceSqr
    g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor() { return GImGui->MouseCursor; }

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type) { GImGui->MouseCursor = cursor_type; }

void ImGui::CaptureKeyboardFromApp(bool capture) { GImGui->CaptureKeyboardNextFrame = capture ? 1 : 0; }

void ImGui::CaptureMouseFromApp(bool capture) { GImGui->CaptureMouseNextFrame = capture ? 1 : 0; }

bool ImGui::IsItemHovered()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemHoveredAndUsable;
}

bool ImGui::IsItemHoveredRect()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemHoveredRect;
}

bool ImGui::IsItemActive()
{
    ImGuiContext& g = *GImGui;
    if (g.ActiveId) {
        ImGuiWindow* window = GetCurrentWindowRead();
        return g.ActiveId == window->DC.LastItemId;
    }
=======
bool ImGui::IsMouseReleased(int button) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  return g.IO.MouseReleased[button];
}

bool ImGui::IsMouseDoubleClicked(int button) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  return g.IO.MouseDoubleClicked[button];
}

bool ImGui::IsMouseDragging(int button, float lock_threshold) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  if (!g.IO.MouseDown[button])
>>>>>>> master
    return false;
  if (lock_threshold < 0.0f)
    lock_threshold = g.IO.MouseDragThreshold;
  return g.IO.MouseDragMaxDistanceSqr[button] >=
         lock_threshold * lock_threshold;
}

<<<<<<< HEAD
bool ImGui::IsItemClicked(int mouse_button) { return IsMouseClicked(mouse_button) && IsItemHovered(); }

bool ImGui::IsAnyItemHovered() { return GImGui->HoveredId != 0 || GImGui->HoveredIdPreviousFrame != 0; }

bool ImGui::IsAnyItemActive() { return GImGui->ActiveId != 0; }

bool ImGui::IsItemVisible()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImRect       r(window->ClipRect);
    return r.Overlaps(window->DC.LastItemRect);
}

// Allow last item to be overlapped by a subsequent item. Both may be activated
// during the same frame before the later one takes priority.
void ImGui::SetItemAllowOverlap()
{
    ImGuiContext& g                                                            = *GImGui;
    if (g.HoveredId == g.CurrentWindow->DC.LastItemId) g.HoveredIdAllowOverlap = true;
    if (g.ActiveId == g.CurrentWindow->DC.LastItemId) g.ActiveIdAllowOverlap   = true;
=======
ImVec2 ImGui::GetMousePos() { return GImGui->IO.MousePos; }

// NB: prefer to call right after BeginPopup(). At the time Selectable/MenuItem
// is activated, the popup is already closed!
ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup() {
  ImGuiContext &g = *GImGui;
  if (g.CurrentPopupStack.Size > 0)
    return g.OpenPopupStack[g.CurrentPopupStack.Size - 1].MousePosOnOpen;
  return g.IO.MousePos;
}

ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  if (lock_threshold < 0.0f)
    lock_threshold = g.IO.MouseDragThreshold;
  if (g.IO.MouseDown[button])
    if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
      return g.IO.MousePos - g.IO.MouseClickedPos[button]; // Assume we can only
                                                           // get active with
                                                           // left-mouse button
                                                           // (at the moment).
  return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
  // NB: We don't need to reset g.IO.MouseDragMaxDistanceSqr
  g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor() { return GImGui->MouseCursor; }

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type) {
  GImGui->MouseCursor = cursor_type;
}

void ImGui::CaptureKeyboardFromApp(bool capture) {
  GImGui->CaptureKeyboardNextFrame = capture ? 1 : 0;
>>>>>>> master
}

void ImGui::CaptureMouseFromApp(bool capture) {
  GImGui->CaptureMouseNextFrame = capture ? 1 : 0;
}

bool ImGui::IsItemHovered() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.LastItemHoveredAndUsable;
}

bool ImGui::IsItemHoveredRect() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.LastItemHoveredRect;
}

<<<<<<< HEAD
ImVec2 ImGui::CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge, float outward)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImRect       rect   = window->DC.LastItemRect;
    rect.Expand(outward);
    return rect.GetClosestPoint(pos, on_edge);
}

// Tooltip is stored and turned into a BeginTooltip()/EndTooltip() sequence at
// the end of the frame. Each call override previous value.
void ImGui::SetTooltipV(const char* fmt, va_list args)
{
    ImGuiContext& g = *GImGui;
    ImFormatStringV(g.Tooltip, IM_ARRAYSIZE(g.Tooltip), fmt, args);
=======
bool ImGui::IsItemActive() {
  ImGuiContext &g = *GImGui;
  if (g.ActiveId) {
    ImGuiWindow *window = GetCurrentWindowRead();
    return g.ActiveId == window->DC.LastItemId;
  }
  return false;
}

bool ImGui::IsItemClicked(int mouse_button) {
  return IsMouseClicked(mouse_button) && IsItemHovered();
>>>>>>> master
}

bool ImGui::IsAnyItemHovered() {
  return GImGui->HoveredId != 0 || GImGui->HoveredIdPreviousFrame != 0;
}

bool ImGui::IsAnyItemActive() { return GImGui->ActiveId != 0; }

bool ImGui::IsItemVisible() {
  ImGuiWindow *window = GetCurrentWindowRead();
  ImRect r(window->ClipRect);
  return r.Overlaps(window->DC.LastItemRect);
}

<<<<<<< HEAD
void ImGui::BeginTooltip()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("##Tooltip", NULL, flags);
}

void ImGui::EndTooltip()
{
    IM_ASSERT(
        GetCurrentWindowRead()->Flags & ImGuiWindowFlags_Tooltip);  // Mismatched BeginTooltip()/EndTooltip() calls
    ImGui::End();
=======
// Allow last item to be overlapped by a subsequent item. Both may be activated
// during the same frame before the later one takes priority.
void ImGui::SetItemAllowOverlap() {
  ImGuiContext &g = *GImGui;
  if (g.HoveredId == g.CurrentWindow->DC.LastItemId)
    g.HoveredIdAllowOverlap = true;
  if (g.ActiveId == g.CurrentWindow->DC.LastItemId)
    g.ActiveIdAllowOverlap = true;
}

ImVec2 ImGui::GetItemRectMin() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.LastItemRect.Min;
>>>>>>> master
}

ImVec2 ImGui::GetItemRectMax() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.LastItemRect.Max;
}

ImVec2 ImGui::GetItemRectSize() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.LastItemRect.GetSize();
}

ImVec2 ImGui::CalcItemRectClosestPoint(const ImVec2 &pos, bool on_edge,
                                       float outward) {
  ImGuiWindow *window = GetCurrentWindowRead();
  ImRect rect = window->DC.LastItemRect;
  rect.Expand(outward);
  return rect.GetClosestPoint(pos, on_edge);
}

// Tooltip is stored and turned into a BeginTooltip()/EndTooltip() sequence at
// the end of the frame. Each call override previous value.
void ImGui::SetTooltipV(const char *fmt, va_list args) {
  ImGuiContext &g = *GImGui;
  ImFormatStringV(g.Tooltip, IM_ARRAYSIZE(g.Tooltip), fmt, args);
}

void ImGui::SetTooltip(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  SetTooltipV(fmt, args);
  va_end(args);
}

static ImRect GetVisibleRect() {
  ImGuiContext &g = *GImGui;
  if (g.IO.DisplayVisibleMin.x != g.IO.DisplayVisibleMax.x &&
      g.IO.DisplayVisibleMin.y != g.IO.DisplayVisibleMax.y)
    return ImRect(g.IO.DisplayVisibleMin, g.IO.DisplayVisibleMax);
  return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

void ImGui::BeginTooltip() {
  ImGuiWindowFlags flags =
      ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
  ImGui::Begin("##Tooltip", NULL, flags);
}

void ImGui::EndTooltip() {
  IM_ASSERT(
      GetCurrentWindowRead()->Flags &
      ImGuiWindowFlags_Tooltip); // Mismatched BeginTooltip()/EndTooltip() calls
  ImGui::End();
}

static bool IsPopupOpen(ImGuiID id) {
  ImGuiContext &g = *GImGui;
  return g.OpenPopupStack.Size > g.CurrentPopupStack.Size &&
         g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == id;
}

// Mark popup as open (toggle toward open state).
// Popups are closed when user click outside, or activate a pressable item, or
// CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block.
// Popup identifiers are relative to the current ID-stack (so OpenPopup and
// BeginPopup needs to be at the same level).
// One open popup per level of the popup hierarchy (NB: when assigning we reset
// the Window member of ImGuiPopupRef to NULL)
<<<<<<< HEAD
void ImGui::OpenPopupEx(const char* str_id, bool reopen_existing)
{
    ImGuiContext& g                  = *GImGui;
    ImGuiWindow*  window             = g.CurrentWindow;
    ImGuiID       id                 = window->GetID(str_id);
    int           current_stack_size = g.CurrentPopupStack.Size;
    ImGuiPopupRef popup_ref          = ImGuiPopupRef(id,
        window,
        window->GetID("##menus"),
        g.IO.MousePos);  // Tagged as new ref because constructor
                         // sets Window to NULL (we are passing the
                         // ParentWindow info here)
    if (g.OpenPopupStack.Size < current_stack_size + 1)
        g.OpenPopupStack.push_back(popup_ref);
    else if (reopen_existing || g.OpenPopupStack[current_stack_size].PopupId != id)
    {
        g.OpenPopupStack.resize(current_stack_size + 1);
        g.OpenPopupStack[current_stack_size] = popup_ref;
=======
void ImGui::OpenPopupEx(const char *str_id, bool reopen_existing) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;
  ImGuiID id = window->GetID(str_id);
  int current_stack_size = g.CurrentPopupStack.Size;
  ImGuiPopupRef popup_ref =
      ImGuiPopupRef(id, window, window->GetID("##menus"),
                    g.IO.MousePos); // Tagged as new ref because constructor
                                    // sets Window to NULL (we are passing the
                                    // ParentWindow info here)
  if (g.OpenPopupStack.Size < current_stack_size + 1)
    g.OpenPopupStack.push_back(popup_ref);
  else if (reopen_existing ||
           g.OpenPopupStack[current_stack_size].PopupId != id) {
    g.OpenPopupStack.resize(current_stack_size + 1);
    g.OpenPopupStack[current_stack_size] = popup_ref;
  }
}

void ImGui::OpenPopup(const char *str_id) { ImGui::OpenPopupEx(str_id, false); }

static void CloseInactivePopups() {
  ImGuiContext &g = *GImGui;
  if (g.OpenPopupStack.empty())
    return;

  // When popups are stacked, clicking on a lower level popups puts focus back
  // to it and close popups above it.
  // Don't close our own child popup windows
  int n = 0;
  if (g.FocusedWindow) {
    for (n = 0; n < g.OpenPopupStack.Size; n++) {
      ImGuiPopupRef &popup = g.OpenPopupStack[n];
      if (!popup.Window)
        continue;
      IM_ASSERT((popup.Window->Flags & ImGuiWindowFlags_Popup) != 0);
      if (popup.Window->Flags & ImGuiWindowFlags_ChildWindow)
        continue;

      bool has_focus = false;
      for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
        has_focus = (g.OpenPopupStack[m].Window &&
                     g.OpenPopupStack[m].Window->RootWindow ==
                         g.FocusedWindow->RootWindow);
      if (!has_focus)
        break;
>>>>>>> master
    }
  }
  if (n < g.OpenPopupStack.Size) // This test is not required but it allows to
                                 // set a useful breakpoint on the line below
    g.OpenPopupStack.resize(n);
}

<<<<<<< HEAD
void ImGui::OpenPopup(const char* str_id) { ImGui::OpenPopupEx(str_id, false); }

static void CloseInactivePopups()
{
    ImGuiContext& g = *GImGui;
    if (g.OpenPopupStack.empty()) return;

    // When popups are stacked, clicking on a lower level popups puts focus back
    // to it and close popups above it.
    // Don't close our own child popup windows
    int n = 0;
    if (g.FocusedWindow) {
        for (n = 0; n < g.OpenPopupStack.Size; n++) {
            ImGuiPopupRef& popup = g.OpenPopupStack[n];
            if (!popup.Window) continue;
            IM_ASSERT((popup.Window->Flags & ImGuiWindowFlags_Popup) != 0);
            if (popup.Window->Flags & ImGuiWindowFlags_ChildWindow) continue;

            bool has_focus = false;
            for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
                has_focus = (g.OpenPopupStack[m].Window &&
                             g.OpenPopupStack[m].Window->RootWindow == g.FocusedWindow->RootWindow);
            if (!has_focus) break;
        }
    }
    if (n < g.OpenPopupStack.Size)  // This test is not required but it allows to
                                    // set a useful breakpoint on the line below
        g.OpenPopupStack.resize(n);
}

static ImGuiWindow* GetFrontMostModalRootWindow()
{
    ImGuiContext& g = *GImGui;
    for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
        if (ImGuiWindow* front_most_popup = g.OpenPopupStack.Data[n].Window)
            if (front_most_popup->Flags & ImGuiWindowFlags_Modal) return front_most_popup;
    return NULL;
}

static void ClosePopupToLevel(int remaining)
{
    ImGuiContext& g = *GImGui;
    if (remaining > 0)
        ImGui::FocusWindow(g.OpenPopupStack[remaining - 1].Window);
    else
        ImGui::FocusWindow(g.OpenPopupStack[0].ParentWindow);
    g.OpenPopupStack.resize(remaining);
}

static void ClosePopup(ImGuiID id)
{
    if (!IsPopupOpen(id)) return;
    ImGuiContext& g = *GImGui;
    ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}

// Close the popup we have begin-ed into.
void ImGui::CloseCurrentPopup()
{
    ImGuiContext& g         = *GImGui;
    int           popup_idx = g.CurrentPopupStack.Size - 1;
    if (popup_idx < 0 || popup_idx > g.OpenPopupStack.Size ||
        g.CurrentPopupStack[popup_idx].PopupId != g.OpenPopupStack[popup_idx].PopupId)
        return;
    while (popup_idx > 0 && g.OpenPopupStack[popup_idx].Window &&
           (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
        popup_idx--;
    ClosePopupToLevel(popup_idx);
}

static inline void ClearSetNextWindowData()
{
    ImGuiContext& g        = *GImGui;
    g.SetNextWindowPosCond = g.SetNextWindowSizeCond = g.SetNextWindowContentSizeCond = g.SetNextWindowCollapsedCond =
        0;
    g.SetNextWindowSizeConstraint = g.SetNextWindowFocus = false;
}

static bool BeginPopupEx(const char* str_id, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;
    const ImGuiID id     = window->GetID(str_id);
    if (!IsPopupOpen(id)) {
        ClearSetNextWindowData();  // We behave like Begin() and need to consume
                                   // those values
        return false;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGuiWindowFlags flags = extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize;

    char name[20];
    if (flags & ImGuiWindowFlags_ChildMenu)
        ImFormatString(name,
            IM_ARRAYSIZE(name),
            "##menu_%d",
            g.CurrentPopupStack.Size);  // Recycle windows based on depth
    else
        ImFormatString(name,
            IM_ARRAYSIZE(name),
            "##popup_%08x",
            id);  // Not recycling, so we can close/open during the same frame

    bool is_open = ImGui::Begin(name, NULL, flags);
    if (!(window->Flags & ImGuiWindowFlags_ShowBorders)) g.CurrentWindow->Flags &= ~ImGuiWindowFlags_ShowBorders;
    if (!is_open)  // NB: is_open can be 'false' when the popup is completely
                   // clipped (e.g. zero size display)
        ImGui::EndPopup();
=======
static ImGuiWindow *GetFrontMostModalRootWindow() {
  ImGuiContext &g = *GImGui;
  for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
    if (ImGuiWindow *front_most_popup = g.OpenPopupStack.Data[n].Window)
      if (front_most_popup->Flags & ImGuiWindowFlags_Modal)
        return front_most_popup;
  return NULL;
}

static void ClosePopupToLevel(int remaining) {
  ImGuiContext &g = *GImGui;
  if (remaining > 0)
    ImGui::FocusWindow(g.OpenPopupStack[remaining - 1].Window);
  else
    ImGui::FocusWindow(g.OpenPopupStack[0].ParentWindow);
  g.OpenPopupStack.resize(remaining);
}

static void ClosePopup(ImGuiID id) {
  if (!IsPopupOpen(id))
    return;
  ImGuiContext &g = *GImGui;
  ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}

// Close the popup we have begin-ed into.
void ImGui::CloseCurrentPopup() {
  ImGuiContext &g = *GImGui;
  int popup_idx = g.CurrentPopupStack.Size - 1;
  if (popup_idx < 0 || popup_idx > g.OpenPopupStack.Size ||
      g.CurrentPopupStack[popup_idx].PopupId !=
          g.OpenPopupStack[popup_idx].PopupId)
    return;
  while (
      popup_idx > 0 && g.OpenPopupStack[popup_idx].Window &&
      (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
    popup_idx--;
  ClosePopupToLevel(popup_idx);
}

static inline void ClearSetNextWindowData() {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowPosCond = g.SetNextWindowSizeCond =
      g.SetNextWindowContentSizeCond = g.SetNextWindowCollapsedCond = 0;
  g.SetNextWindowSizeConstraint = g.SetNextWindowFocus = false;
}

static bool BeginPopupEx(const char *str_id, ImGuiWindowFlags extra_flags) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;
  const ImGuiID id = window->GetID(str_id);
  if (!IsPopupOpen(id)) {
    ClearSetNextWindowData(); // We behave like Begin() and need to consume
                              // those values
    return false;
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGuiWindowFlags flags =
      extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

  char name[20];
  if (flags & ImGuiWindowFlags_ChildMenu)
    ImFormatString(name, IM_ARRAYSIZE(name), "##menu_%d",
                   g.CurrentPopupStack.Size); // Recycle windows based on depth
  else
    ImFormatString(
        name, IM_ARRAYSIZE(name), "##popup_%08x",
        id); // Not recycling, so we can close/open during the same frame

  bool is_open = ImGui::Begin(name, NULL, flags);
  if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
    g.CurrentWindow->Flags &= ~ImGuiWindowFlags_ShowBorders;
  if (!is_open) // NB: is_open can be 'false' when the popup is completely
                // clipped (e.g. zero size display)
    ImGui::EndPopup();
>>>>>>> master

  return is_open;
}

<<<<<<< HEAD
bool ImGui::BeginPopup(const char* str_id)
{
    if (GImGui->OpenPopupStack.Size <= GImGui->CurrentPopupStack.Size)  // Early out for performance
    {
        ClearSetNextWindowData();  // We behave like Begin() and need to consume
                                   // those values
        return false;
    }
    return BeginPopupEx(str_id, ImGuiWindowFlags_ShowBorders);
}

bool ImGui::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;
    const ImGuiID id     = window->GetID(name);
    if (!IsPopupOpen(id)) {
        ClearSetNextWindowData();  // We behave like Begin() and need to consume
                                   // those values
        return false;
    }

    ImGuiWindowFlags flags = extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_Modal |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
    bool is_open = ImGui::Begin(name, p_open, flags);
    if (!is_open || (p_open && !*p_open))  // NB: is_open can be 'false' when the
                                           // popup is completely clipped (e.g.
                                           // zero size display)
    {
        ImGui::EndPopup();
        if (is_open) ClosePopup(id);
        return false;
    }
=======
bool ImGui::BeginPopup(const char *str_id) {
  if (GImGui->OpenPopupStack.Size <=
      GImGui->CurrentPopupStack.Size) // Early out for performance
  {
    ClearSetNextWindowData(); // We behave like Begin() and need to consume
                              // those values
    return false;
  }
  return BeginPopupEx(str_id, ImGuiWindowFlags_ShowBorders);
}

bool ImGui::BeginPopupModal(const char *name, bool *p_open,
                            ImGuiWindowFlags extra_flags) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;
  const ImGuiID id = window->GetID(name);
  if (!IsPopupOpen(id)) {
    ClearSetNextWindowData(); // We behave like Begin() and need to consume
                              // those values
    return false;
  }

  ImGuiWindowFlags flags =
      extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_Modal |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
  bool is_open = ImGui::Begin(name, p_open, flags);
  if (!is_open || (p_open && !*p_open)) // NB: is_open can be 'false' when the
                                        // popup is completely clipped (e.g.
                                        // zero size display)
  {
    ImGui::EndPopup();
    if (is_open)
      ClosePopup(id);
    return false;
  }
>>>>>>> master

  return is_open;
}

<<<<<<< HEAD
void ImGui::EndPopup()
{
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(window->Flags & ImGuiWindowFlags_Popup);  // Mismatched BeginPopup()/EndPopup() calls
    IM_ASSERT(GImGui->CurrentPopupStack.Size > 0);
    ImGui::End();
    if (!(window->Flags & ImGuiWindowFlags_Modal)) ImGui::PopStyleVar();
=======
void ImGui::EndPopup() {
  ImGuiWindow *window = GetCurrentWindow();
  IM_ASSERT(window->Flags &
            ImGuiWindowFlags_Popup); // Mismatched BeginPopup()/EndPopup() calls
  IM_ASSERT(GImGui->CurrentPopupStack.Size > 0);
  ImGui::End();
  if (!(window->Flags & ImGuiWindowFlags_Modal))
    ImGui::PopStyleVar();
>>>>>>> master
}

// This is a helper to handle the most simple case of associating one named
// popup to one given widget.
// 1. If you have many possible popups (for different "instances" of a same
// widget, or for wholly different widgets), you may be better off handling
//    this yourself so you can store data relative to the widget that opened the
//    popup instead of choosing different popup identifiers.
// 2. If you want right-clicking on the same item to reopen the popup at new
// location, use the same code replacing IsItemHovered() with
// IsItemHoveredRect()
//    and passing true to the OpenPopupEx().
//    Because: hovering an item in a window below the popup won't normally
//    trigger is hovering behavior/coloring. The pattern of ignoring the fact
//    that
//    the item isn't interactable (because it is blocked by the active popup)
//    may useful in some situation when e.g. large canvas as one item, content
//    of menu
//    driven by click position.
<<<<<<< HEAD
bool ImGui::BeginPopupContextItem(const char* str_id, int mouse_button)
{
    if (IsItemHovered() && IsMouseClicked(mouse_button)) OpenPopupEx(str_id, false);
    return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextWindow(bool also_over_items, const char* str_id, int mouse_button)
{
    if (!str_id) str_id = "window_context_menu";
    if (IsMouseHoveringWindow() && IsMouseClicked(mouse_button))
        if (also_over_items || !IsAnyItemHovered()) OpenPopupEx(str_id, true);
    return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextVoid(const char* str_id, int mouse_button)
{
    if (!str_id) str_id = "void_context_menu";
    if (!IsMouseHoveringAnyWindow() && IsMouseClicked(mouse_button)) OpenPopupEx(str_id, true);
    return BeginPopup(str_id);
}

static bool BeginChildEx(
    const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow*     window = ImGui::GetCurrentWindow();
    ImGuiWindowFlags flags  = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

    const ImVec2 content_avail = ImGui::GetContentRegionAvail();
    ImVec2       size          = ImFloor(size_arg);
    if (size.x <= 0.0f) {
        if (size.x == 0.0f) flags |= ImGuiWindowFlags_ChildWindowAutoFitX;
        size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x);  // Arbitrary minimum zero-ish child size of 4.0f
                                                                // (0.0f causing too much issues)
    }
    if (size.y <= 0.0f) {
        if (size.y == 0.0f) flags |= ImGuiWindowFlags_ChildWindowAutoFitY;
        size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
    }
    if (border) flags |= ImGuiWindowFlags_ShowBorders;
    flags |= extra_flags;
=======
bool ImGui::BeginPopupContextItem(const char *str_id, int mouse_button) {
  if (IsItemHovered() && IsMouseClicked(mouse_button))
    OpenPopupEx(str_id, false);
  return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextWindow(bool also_over_items, const char *str_id,
                                    int mouse_button) {
  if (!str_id)
    str_id = "window_context_menu";
  if (IsMouseHoveringWindow() && IsMouseClicked(mouse_button))
    if (also_over_items || !IsAnyItemHovered())
      OpenPopupEx(str_id, true);
  return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextVoid(const char *str_id, int mouse_button) {
  if (!str_id)
    str_id = "void_context_menu";
  if (!IsMouseHoveringAnyWindow() && IsMouseClicked(mouse_button))
    OpenPopupEx(str_id, true);
  return BeginPopup(str_id);
}

static bool BeginChildEx(const char *name, ImGuiID id, const ImVec2 &size_arg,
                         bool border, ImGuiWindowFlags extra_flags) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

  const ImVec2 content_avail = ImGui::GetContentRegionAvail();
  ImVec2 size = ImFloor(size_arg);
  if (size.x <= 0.0f) {
    if (size.x == 0.0f)
      flags |= ImGuiWindowFlags_ChildWindowAutoFitX;
    size.x = ImMax(content_avail.x, 4.0f) -
             fabsf(size.x); // Arbitrary minimum zero-ish child size of 4.0f
                            // (0.0f causing too much issues)
  }
  if (size.y <= 0.0f) {
    if (size.y == 0.0f)
      flags |= ImGuiWindowFlags_ChildWindowAutoFitY;
    size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
  }
  if (border)
    flags |= ImGuiWindowFlags_ShowBorders;
  flags |= extra_flags;
>>>>>>> master

  char title[256];
  if (name)
    ImFormatString(title, IM_ARRAYSIZE(title), "%s.%s.%08X", window->Name, name,
                   id);
  else
    ImFormatString(title, IM_ARRAYSIZE(title), "%s.%08X", window->Name, id);

  bool ret = ImGui::Begin(title, NULL, size, -1.0f, flags);

  if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
    ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

  return ret;
}

bool ImGui::BeginChild(const char *str_id, const ImVec2 &size_arg, bool border,
                       ImGuiWindowFlags extra_flags) {
  ImGuiWindow *window = GetCurrentWindow();
  return BeginChildEx(str_id, window->GetID(str_id), size_arg, border,
                      extra_flags);
}

bool ImGui::BeginChild(ImGuiID id, const ImVec2 &size_arg, bool border,
                       ImGuiWindowFlags extra_flags) {
  return BeginChildEx(NULL, id, size_arg, border, extra_flags);
}

void ImGui::EndChild() {
  ImGuiWindow *window = GetCurrentWindow();

<<<<<<< HEAD
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);  // Mismatched BeginChild()/EndChild()
                                                              // callss
    if ((window->Flags & ImGuiWindowFlags_ComboBox) || window->BeginCount > 1) {
        ImGui::End();
    }
    else
    {
        // When using auto-filling child window, we don't provide full width/height
        // to ItemSize so that it doesn't feed back into automatic size-fitting.
        ImVec2 sz = GetWindowSize();
        if (window->Flags & ImGuiWindowFlags_ChildWindowAutoFitX)  // Arbitrary minimum zero-ish
                                                                   // child size of 4.0f causes less
                                                                   // trouble than a 0.0f
            sz.x                                                       = ImMax(4.0f, sz.x);
        if (window->Flags & ImGuiWindowFlags_ChildWindowAutoFitY) sz.y = ImMax(4.0f, sz.y);
=======
  IM_ASSERT(window->Flags &
            ImGuiWindowFlags_ChildWindow); // Mismatched BeginChild()/EndChild()
                                           // callss
  if ((window->Flags & ImGuiWindowFlags_ComboBox) || window->BeginCount > 1) {
    ImGui::End();
  } else {
    // When using auto-filling child window, we don't provide full width/height
    // to ItemSize so that it doesn't feed back into automatic size-fitting.
    ImVec2 sz = GetWindowSize();
    if (window->Flags &
        ImGuiWindowFlags_ChildWindowAutoFitX) // Arbitrary minimum zero-ish
                                              // child size of 4.0f causes less
                                              // trouble than a 0.0f
      sz.x = ImMax(4.0f, sz.x);
    if (window->Flags & ImGuiWindowFlags_ChildWindowAutoFitY)
      sz.y = ImMax(4.0f, sz.y);
>>>>>>> master

    ImGui::End();

    window = GetCurrentWindow();
    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + sz);
    ItemSize(sz);
    ItemAdd(bb, NULL);
  }
}

// Helper to create a child window / scrolling region that looks like a normal
// widget frame.
<<<<<<< HEAD
bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags)
{
    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_FrameBg]);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, style.FrameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    return ImGui::BeginChild(id,
        size,
        (g.CurrentWindow->Flags & ImGuiWindowFlags_ShowBorders) ? true : false,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
=======
bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2 &size,
                            ImGuiWindowFlags extra_flags) {
  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_FrameBg]);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, style.FrameRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
  return ImGui::BeginChild(
      id, size,
      (g.CurrentWindow->Flags & ImGuiWindowFlags_ShowBorders) ? true : false,
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding |
          extra_flags);
>>>>>>> master
}

void ImGui::EndChildFrame() {
  ImGui::EndChild();
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();
}

// Save and compare stack sizes on Begin()/End() to detect usage errors
<<<<<<< HEAD
static void CheckStacksSize(ImGuiWindow* window, bool write)
{
    // NOT checking: DC.ItemWidth, DC.AllowKeyboardFocus, DC.ButtonRepeat,
    // DC.TextWrapPos (per window) to allow user to conveniently push once and not
    // pop (they are cleared on Begin)
    ImGuiContext& g        = *GImGui;
    int*          p_backup = &window->DC.StackSizesBackup[0];
    {
        int current = window->IDStack.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "PushID/PopID or TreeNode/TreePop Mismatch!");
        p_backup++;
    }  // Too few or too many PopID()/TreePop()
    {
        int current = window->DC.GroupStack.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "BeginGroup/EndGroup Mismatch!");
        p_backup++;
    }  // Too few or too many EndGroup()
    {
        int current = g.CurrentPopupStack.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "BeginMenu/EndMenu or BeginPopup/EndPopup Mismatch");
        p_backup++;
    }  // Too few or too many EndMenu()/EndPopup()
    {
        int current = g.ColorModifiers.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "PushStyleColor/PopStyleColor Mismatch!");
        p_backup++;
    }  // Too few or too many PopStyleColor()
    {
        int current = g.StyleModifiers.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "PushStyleVar/PopStyleVar Mismatch!");
        p_backup++;
    }  // Too few or too many PopStyleVar()
    {
        int current = g.FontStack.Size;
        if (write)
            *p_backup = current;
        else
            IM_ASSERT(*p_backup == current && "PushFont/PopFont Mismatch!");
        p_backup++;
    }  // Too few or too many PopFont()
    IM_ASSERT(p_backup == window->DC.StackSizesBackup + IM_ARRAYSIZE(window->DC.StackSizesBackup));
}

static ImVec2 FindBestPopupWindowPos(const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& r_inner)
{
    const ImGuiStyle& style = GImGui->Style;

    // Clamp into visible area while not overlapping the cursor. Safety padding is
    // optional if our popup size won't fit without it.
    ImVec2 safe_padding = style.DisplaySafeAreaPadding;
    ImRect r_outer(GetVisibleRect());
    r_outer.Reduce(ImVec2((size.x - r_outer.GetWidth() > safe_padding.x * 2) ? safe_padding.x : 0.0f,
        (size.y - r_outer.GetHeight() > safe_padding.y * 2) ? safe_padding.y : 0.0f));
    ImVec2 base_pos_clamped = ImClamp(base_pos, r_outer.Min, r_outer.Max - size);

    for (int n = (*last_dir != -1) ? -1 : 0; n < 4; n++)  // Last, Right, down, up, left. (Favor last used direction).
    {
        const int dir = (n == -1) ? *last_dir : n;
        ImRect    rect(dir == 0 ? r_inner.Max.x : r_outer.Min.x,
            dir == 1 ? r_inner.Max.y : r_outer.Min.y,
            dir == 3 ? r_inner.Min.x : r_outer.Max.x,
            dir == 2 ? r_inner.Min.y : r_outer.Max.y);
        if (rect.GetWidth() < size.x || rect.GetHeight() < size.y) continue;
        *last_dir = dir;
        return ImVec2(dir == 0 ? r_inner.Max.x : dir == 3 ? r_inner.Min.x - size.x : base_pos_clamped.x,
            dir == 1 ? r_inner.Max.y : dir == 2 ? r_inner.Min.y - size.y : base_pos_clamped.y);
    }

    // Fallback, try to keep within display
    *last_dir  = -1;
    ImVec2 pos = base_pos;
    pos.x      = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
    pos.y      = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
    return pos;
}

ImGuiWindow* ImGui::FindWindowByName(const char* name)
{
    // FIXME-OPT: Store sorted hashes -> pointers so we can do a bissection in a
    // contiguous block
    ImGuiContext& g  = *GImGui;
    ImGuiID       id = ImHash(name, 0);
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i]->ID == id) return g.Windows[i];
    return NULL;
}

static ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;

    // Create window the first time
    ImGuiWindow* window = (ImGuiWindow*)ImGui::MemAlloc(sizeof(ImGuiWindow));
    IM_PLACEMENT_NEW(window) ImGuiWindow(name);
    window->Flags = flags;

    if (flags & ImGuiWindowFlags_NoSavedSettings) {
        // User can disable loading and saving of settings. Tooltip and child
        // windows also don't store settings.
        window->Size = window->SizeFull = size;
    }
    else
    {
        // Retrieve settings from .ini file
        // Use SetWindowPos() or SetNextWindowPos() with the appropriate condition
        // flag to change the initial position of a window.
        window->PosFloat = ImVec2(60, 60);
        window->Pos      = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

        ImGuiIniData* settings = FindWindowSettings(name);
        if (!settings) {
            settings = AddWindowSettings(name);
        }
        else
        {
            window->SetWindowPosAllowFlags &= ~ImGuiSetCond_FirstUseEver;
            window->SetWindowSizeAllowFlags &= ~ImGuiSetCond_FirstUseEver;
            window->SetWindowCollapsedAllowFlags &= ~ImGuiSetCond_FirstUseEver;
        }

        if (settings->Pos.x != FLT_MAX) {
            window->PosFloat  = settings->Pos;
            window->Pos       = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
            window->Collapsed = settings->Collapsed;
        }

        if (ImLengthSqr(settings->Size) > 0.00001f && !(flags & ImGuiWindowFlags_NoResize)) size = settings->Size;
        window->Size = window->SizeFull = size;
    }

    if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0) {
        window->AutoFitFramesX = window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows                        = false;
    }
    else
    {
        if (window->Size.x <= 0.0f) window->AutoFitFramesX = 2;
        if (window->Size.y <= 0.0f) window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
    }

    if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
        g.Windows.insert(g.Windows.begin(), window);  // Quite slow but rare and only once
=======
static void CheckStacksSize(ImGuiWindow *window, bool write) {
  // NOT checking: DC.ItemWidth, DC.AllowKeyboardFocus, DC.ButtonRepeat,
  // DC.TextWrapPos (per window) to allow user to conveniently push once and not
  // pop (they are cleared on Begin)
  ImGuiContext &g = *GImGui;
  int *p_backup = &window->DC.StackSizesBackup[0];
  {
    int current = window->IDStack.Size;
    if (write)
      *p_backup = current;
    else
      IM_ASSERT(*p_backup == current &&
                "PushID/PopID or TreeNode/TreePop Mismatch!");
    p_backup++;
  } // Too few or too many PopID()/TreePop()
  {
    int current = window->DC.GroupStack.Size;
    if (write)
      *p_backup = current;
    else
      IM_ASSERT(*p_backup == current && "BeginGroup/EndGroup Mismatch!");
    p_backup++;
  } // Too few or too many EndGroup()
  {
    int current = g.CurrentPopupStack.Size;
    if (write)
      *p_backup = current;
>>>>>>> master
    else
      IM_ASSERT(*p_backup == current &&
                "BeginMenu/EndMenu or BeginPopup/EndPopup Mismatch");
    p_backup++;
  } // Too few or too many EndMenu()/EndPopup()
  {
    int current = g.ColorModifiers.Size;
    if (write)
      *p_backup = current;
    else
      IM_ASSERT(*p_backup == current &&
                "PushStyleColor/PopStyleColor Mismatch!");
    p_backup++;
  } // Too few or too many PopStyleColor()
  {
    int current = g.StyleModifiers.Size;
    if (write)
      *p_backup = current;
    else
      IM_ASSERT(*p_backup == current && "PushStyleVar/PopStyleVar Mismatch!");
    p_backup++;
  } // Too few or too many PopStyleVar()
  {
    int current = g.FontStack.Size;
    if (write)
      *p_backup = current;
    else
      IM_ASSERT(*p_backup == current && "PushFont/PopFont Mismatch!");
    p_backup++;
  } // Too few or too many PopFont()
  IM_ASSERT(p_backup ==
            window->DC.StackSizesBackup +
                IM_ARRAYSIZE(window->DC.StackSizesBackup));
}

<<<<<<< HEAD
static void ApplySizeFullWithConstraint(ImGuiWindow* window, ImVec2 new_size)
{
    ImGuiContext& g = *GImGui;
    if (g.SetNextWindowSizeConstraint) {
        // Using -1,-1 on either X/Y axis to preserve the current size.
        ImRect cr  = g.SetNextWindowSizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.SetNextWindowSizeConstraintCallback) {
            ImGuiSizeConstraintCallbackData data;
            data.UserData    = g.SetNextWindowSizeConstraintCallbackUserData;
            data.Pos         = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.SetNextWindowSizeConstraintCallback(&data);
            new_size = data.DesiredSize;
        }
    }
    if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
        new_size     = ImMax(new_size, g.Style.WindowMinSize);
    window->SizeFull = new_size;
=======
static ImVec2 FindBestPopupWindowPos(const ImVec2 &base_pos, const ImVec2 &size,
                                     int *last_dir, const ImRect &r_inner) {
  const ImGuiStyle &style = GImGui->Style;

  // Clamp into visible area while not overlapping the cursor. Safety padding is
  // optional if our popup size won't fit without it.
  ImVec2 safe_padding = style.DisplaySafeAreaPadding;
  ImRect r_outer(GetVisibleRect());
  r_outer.Reduce(ImVec2(
      (size.x - r_outer.GetWidth() > safe_padding.x * 2) ? safe_padding.x
                                                         : 0.0f,
      (size.y - r_outer.GetHeight() > safe_padding.y * 2) ? safe_padding.y
                                                          : 0.0f));
  ImVec2 base_pos_clamped = ImClamp(base_pos, r_outer.Min, r_outer.Max - size);

  for (int n = (*last_dir != -1) ? -1 : 0; n < 4;
       n++) // Last, Right, down, up, left. (Favor last used direction).
  {
    const int dir = (n == -1) ? *last_dir : n;
    ImRect rect(dir == 0 ? r_inner.Max.x : r_outer.Min.x,
                dir == 1 ? r_inner.Max.y : r_outer.Min.y,
                dir == 3 ? r_inner.Min.x : r_outer.Max.x,
                dir == 2 ? r_inner.Min.y : r_outer.Max.y);
    if (rect.GetWidth() < size.x || rect.GetHeight() < size.y)
      continue;
    *last_dir = dir;
    return ImVec2(dir == 0 ? r_inner.Max.x : dir == 3 ? r_inner.Min.x - size.x
                                                      : base_pos_clamped.x,
                  dir == 1 ? r_inner.Max.y : dir == 2 ? r_inner.Min.y - size.y
                                                      : base_pos_clamped.y);
  }

  // Fallback, try to keep within display
  *last_dir = -1;
  ImVec2 pos = base_pos;
  pos.x = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
  pos.y = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
  return pos;
}

ImGuiWindow *ImGui::FindWindowByName(const char *name) {
  // FIXME-OPT: Store sorted hashes -> pointers so we can do a bissection in a
  // contiguous block
  ImGuiContext &g = *GImGui;
  ImGuiID id = ImHash(name, 0);
  for (int i = 0; i < g.Windows.Size; i++)
    if (g.Windows[i]->ID == id)
      return g.Windows[i];
  return NULL;
}

static ImGuiWindow *CreateNewWindow(const char *name, ImVec2 size,
                                    ImGuiWindowFlags flags) {
  ImGuiContext &g = *GImGui;

  // Create window the first time
  ImGuiWindow *window = (ImGuiWindow *)ImGui::MemAlloc(sizeof(ImGuiWindow));
  IM_PLACEMENT_NEW(window) ImGuiWindow(name);
  window->Flags = flags;

  if (flags & ImGuiWindowFlags_NoSavedSettings) {
    // User can disable loading and saving of settings. Tooltip and child
    // windows also don't store settings.
    window->Size = window->SizeFull = size;
  } else {
    // Retrieve settings from .ini file
    // Use SetWindowPos() or SetNextWindowPos() with the appropriate condition
    // flag to change the initial position of a window.
    window->PosFloat = ImVec2(60, 60);
    window->Pos =
        ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

    ImGuiIniData *settings = FindWindowSettings(name);
    if (!settings) {
      settings = AddWindowSettings(name);
    } else {
      window->SetWindowPosAllowFlags &= ~ImGuiSetCond_FirstUseEver;
      window->SetWindowSizeAllowFlags &= ~ImGuiSetCond_FirstUseEver;
      window->SetWindowCollapsedAllowFlags &= ~ImGuiSetCond_FirstUseEver;
    }

    if (settings->Pos.x != FLT_MAX) {
      window->PosFloat = settings->Pos;
      window->Pos = ImVec2((float)(int)window->PosFloat.x,
                           (float)(int)window->PosFloat.y);
      window->Collapsed = settings->Collapsed;
    }

    if (ImLengthSqr(settings->Size) > 0.00001f &&
        !(flags & ImGuiWindowFlags_NoResize))
      size = settings->Size;
    window->Size = window->SizeFull = size;
  }

  if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0) {
    window->AutoFitFramesX = window->AutoFitFramesY = 2;
    window->AutoFitOnlyGrows = false;
  } else {
    if (window->Size.x <= 0.0f)
      window->AutoFitFramesX = 2;
    if (window->Size.y <= 0.0f)
      window->AutoFitFramesY = 2;
    window->AutoFitOnlyGrows =
        (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
  }

  if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
    g.Windows.insert(g.Windows.begin(),
                     window); // Quite slow but rare and only once
  else
    g.Windows.push_back(window);
  return window;
}

static void ApplySizeFullWithConstraint(ImGuiWindow *window, ImVec2 new_size) {
  ImGuiContext &g = *GImGui;
  if (g.SetNextWindowSizeConstraint) {
    // Using -1,-1 on either X/Y axis to preserve the current size.
    ImRect cr = g.SetNextWindowSizeConstraintRect;
    new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0)
                     ? ImClamp(new_size.x, cr.Min.x, cr.Max.x)
                     : window->SizeFull.x;
    new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0)
                     ? ImClamp(new_size.y, cr.Min.y, cr.Max.y)
                     : window->SizeFull.y;
    if (g.SetNextWindowSizeConstraintCallback) {
      ImGuiSizeConstraintCallbackData data;
      data.UserData = g.SetNextWindowSizeConstraintCallbackUserData;
      data.Pos = window->Pos;
      data.CurrentSize = window->SizeFull;
      data.DesiredSize = new_size;
      g.SetNextWindowSizeConstraintCallback(&data);
      new_size = data.DesiredSize;
    }
  }
  if (!(window->Flags &
        (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
    new_size = ImMax(new_size, g.Style.WindowMinSize);
  window->SizeFull = new_size;
>>>>>>> master
}

// Push a new ImGui window to add widgets to.
// - A default window called "Debug" is automatically stacked at the beginning
// of every frame so you can use widgets without explicitly calling a Begin/End
// pair.
// - Begin/End can be called multiple times during the frame with the same
// window name to append content.
// - 'size_on_first_use' for a regular window denote the initial size for
// first-time creation (no saved data) and isn't that useful. Use
// SetNextWindowSize() prior to calling Begin() for more flexible window
// manipulation.
// - The window name is used as a unique identifier to preserve window
// information across frames (and save rudimentary information to the .ini
// file).
//   You can use the "##" or "###" markers to use the same label with different
//   id, or same id with different label. See documentation at the top of this
//   file.
// - Return false when window is collapsed, so you can early out in your code.
// You always need to call ImGui::End() even if false is returned.
// - Passing 'bool* p_open' displays a Close button on the upper-right corner of
// the window, the pointed value will be set to false when the button is
// pressed.
// - Passing non-zero 'size' is roughly equivalent to calling
// SetNextWindowSize(size, ImGuiSetCond_FirstUseEver) prior to calling Begin().
<<<<<<< HEAD
bool ImGui::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    return ImGui::Begin(name, p_open, ImVec2(0.f, 0.f), -1.0f, flags);
}

bool ImGui::Begin(
    const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha, ImGuiWindowFlags flags)
{
    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    IM_ASSERT(name != NULL);                       // Window name required
    IM_ASSERT(g.Initialized);                      // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);  // Called ImGui::Render() or
                                                   // ImGui::EndFrame() and haven't
                                                   // called ImGui::NewFrame()
                                                   // again yet

    if (flags & ImGuiWindowFlags_NoInputs) flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    // Find or create
    bool         window_is_new = false;
    ImGuiWindow* window        = FindWindowByName(name);
    if (!window) {
        window        = CreateNewWindow(name, size_on_first_use, flags);
        window_is_new = true;
=======
bool ImGui::Begin(const char *name, bool *p_open, ImGuiWindowFlags flags) {
  return ImGui::Begin(name, p_open, ImVec2(0.f, 0.f), -1.0f, flags);
}

bool ImGui::Begin(const char *name, bool *p_open,
                  const ImVec2 &size_on_first_use, float bg_alpha,
                  ImGuiWindowFlags flags) {
  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  IM_ASSERT(name != NULL);  // Window name required
  IM_ASSERT(g.Initialized); // Forgot to call ImGui::NewFrame()
  IM_ASSERT(g.FrameCountEnded != g.FrameCount); // Called ImGui::Render() or
                                                // ImGui::EndFrame() and haven't
                                                // called ImGui::NewFrame()
                                                // again yet

  if (flags & ImGuiWindowFlags_NoInputs)
    flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

  // Find or create
  bool window_is_new = false;
  ImGuiWindow *window = FindWindowByName(name);
  if (!window) {
    window = CreateNewWindow(name, size_on_first_use, flags);
    window_is_new = true;
  }

  const int current_frame = ImGui::GetFrameCount();
  const bool first_begin_of_the_frame =
      (window->LastFrameActive != current_frame);
  if (first_begin_of_the_frame)
    window->Flags = (ImGuiWindowFlags)flags;
  else
    flags = window->Flags;

  // Add to stack
  ImGuiWindow *parent_window =
      !g.CurrentWindowStack.empty() ? g.CurrentWindowStack.back() : NULL;
  g.CurrentWindowStack.push_back(window);
  SetCurrentWindow(window);
  CheckStacksSize(window, true);
  IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));

  bool window_was_active =
      (window->LastFrameActive == current_frame - 1); // Not using !WasActive
                                                      // because the implicit
                                                      // "Debug" window would
                                                      // always toggle off->on
  if (flags & ImGuiWindowFlags_Popup) {
    ImGuiPopupRef &popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
    window_was_active &= (window->PopupId == popup_ref.PopupId);
    window_was_active &= (window == popup_ref.Window);
    popup_ref.Window = window;
    g.CurrentPopupStack.push_back(popup_ref);
    window->PopupId = popup_ref.PopupId;
  }

  const bool window_appearing_after_being_hidden = (window->HiddenFrames == 1);

  // Process SetNextWindow***() calls
  bool window_pos_set_by_api = false, window_size_set_by_api = false;
  if (g.SetNextWindowPosCond) {
    const ImVec2 backup_cursor_pos =
        window->DC.CursorPos; // FIXME: not sure of the exact reason of this
                              // saving/restore anymore :( need to look into
                              // that.
    if (!window_was_active || window_appearing_after_being_hidden)
      window->SetWindowPosAllowFlags |= ImGuiSetCond_Appearing;
    window_pos_set_by_api =
        (window->SetWindowPosAllowFlags & g.SetNextWindowPosCond) != 0;
    if (window_pos_set_by_api &&
        ImLengthSqr(g.SetNextWindowPosVal - ImVec2(-FLT_MAX, -FLT_MAX)) <
            0.001f) {
      window->SetWindowPosCenterWanted =
          true; // May be processed on the next frame if this is our first frame
                // and we are measuring size
      window->SetWindowPosAllowFlags &=
          ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver |
            ImGuiSetCond_Appearing);
    } else {
      SetWindowPos(window, g.SetNextWindowPosVal, g.SetNextWindowPosCond);
>>>>>>> master
    }
    window->DC.CursorPos = backup_cursor_pos;
    g.SetNextWindowPosCond = 0;
  }
  if (g.SetNextWindowSizeCond) {
    if (!window_was_active || window_appearing_after_being_hidden)
      window->SetWindowSizeAllowFlags |= ImGuiSetCond_Appearing;
    window_size_set_by_api =
        (window->SetWindowSizeAllowFlags & g.SetNextWindowSizeCond) != 0;
    SetWindowSize(window, g.SetNextWindowSizeVal, g.SetNextWindowSizeCond);
    g.SetNextWindowSizeCond = 0;
  }
  if (g.SetNextWindowContentSizeCond) {
    window->SizeContentsExplicit = g.SetNextWindowContentSizeVal;
    g.SetNextWindowContentSizeCond = 0;
  } else if (first_begin_of_the_frame) {
    window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
  }
  if (g.SetNextWindowCollapsedCond) {
    if (!window_was_active || window_appearing_after_being_hidden)
      window->SetWindowCollapsedAllowFlags |= ImGuiSetCond_Appearing;
    SetWindowCollapsed(window, g.SetNextWindowCollapsedVal,
                       g.SetNextWindowCollapsedCond);
    g.SetNextWindowCollapsedCond = 0;
  }
  if (g.SetNextWindowFocus) {
    ImGui::SetWindowFocus();
    g.SetNextWindowFocus = false;
  }

<<<<<<< HEAD
    const int  current_frame            = ImGui::GetFrameCount();
    const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
    if (first_begin_of_the_frame)
        window->Flags = (ImGuiWindowFlags)flags;
=======
  // Update known root window (if we are a child window, otherwise window ==
  // window->RootWindow)
  int root_idx, root_non_popup_idx;
  for (root_idx = g.CurrentWindowStack.Size - 1; root_idx > 0; root_idx--)
    if (!(g.CurrentWindowStack[root_idx]->Flags & ImGuiWindowFlags_ChildWindow))
      break;
  for (root_non_popup_idx = root_idx; root_non_popup_idx > 0;
       root_non_popup_idx--)
    if (!(g.CurrentWindowStack[root_non_popup_idx]->Flags &
          (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)))
      break;
  window->ParentWindow = parent_window;
  window->RootWindow = g.CurrentWindowStack[root_idx];
  window->RootNonPopupWindow =
      g.CurrentWindowStack[root_non_popup_idx]; // This is merely for displaying
                                                // the TitleBgActive color.

  // When reusing window again multiple times a frame, just append content
  // (don't need to setup again)
  if (first_begin_of_the_frame) {
    window->Active = true;
    window->IndexWithinParent = 0;
    window->BeginCount = 0;
    window->ClipRect = ImVec4(-FLT_MAX, -FLT_MAX, +FLT_MAX, +FLT_MAX);
    window->LastFrameActive = current_frame;
    window->IDStack.resize(1);

    // Clear draw list, setup texture, outer clipping rectangle
    window->DrawList->Clear();
    window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
    ImRect fullscreen_rect(GetVisibleRect());
    if ((flags & ImGuiWindowFlags_ChildWindow) &&
        !(flags & (ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup)))
      PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max,
                   true);
>>>>>>> master
    else
      PushClipRect(fullscreen_rect.Min, fullscreen_rect.Max, true);

<<<<<<< HEAD
    // Add to stack
    ImGuiWindow* parent_window = !g.CurrentWindowStack.empty() ? g.CurrentWindowStack.back() : NULL;
    g.CurrentWindowStack.push_back(window);
    SetCurrentWindow(window);
    CheckStacksSize(window, true);
    IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));

    bool window_was_active = (window->LastFrameActive == current_frame - 1);  // Not using !WasActive
                                                                              // because the implicit
                                                                              // "Debug" window would
                                                                              // always toggle off->on
    if (flags & ImGuiWindowFlags_Popup) {
        ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
        window_was_active &= (window->PopupId == popup_ref.PopupId);
        window_was_active &= (window == popup_ref.Window);
        popup_ref.Window = window;
        g.CurrentPopupStack.push_back(popup_ref);
        window->PopupId = popup_ref.PopupId;
    }

    const bool window_appearing_after_being_hidden = (window->HiddenFrames == 1);

    // Process SetNextWindow***() calls
    bool window_pos_set_by_api = false, window_size_set_by_api = false;
    if (g.SetNextWindowPosCond) {
        const ImVec2 backup_cursor_pos = window->DC.CursorPos;  // FIXME: not sure of the exact reason of this
                                                                // saving/restore anymore :( need to look into
                                                                // that.
        if (!window_was_active || window_appearing_after_being_hidden)
            window->SetWindowPosAllowFlags |= ImGuiSetCond_Appearing;
        window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.SetNextWindowPosCond) != 0;
        if (window_pos_set_by_api && ImLengthSqr(g.SetNextWindowPosVal - ImVec2(-FLT_MAX, -FLT_MAX)) < 0.001f) {
            window->SetWindowPosCenterWanted = true;  // May be processed on the next frame if this is our first frame
                                                      // and we are measuring size
            window->SetWindowPosAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);
        }
        else
        {
            SetWindowPos(window, g.SetNextWindowPosVal, g.SetNextWindowPosCond);
        }
        window->DC.CursorPos   = backup_cursor_pos;
        g.SetNextWindowPosCond = 0;
    }
    if (g.SetNextWindowSizeCond) {
        if (!window_was_active || window_appearing_after_being_hidden)
            window->SetWindowSizeAllowFlags |= ImGuiSetCond_Appearing;
        window_size_set_by_api = (window->SetWindowSizeAllowFlags & g.SetNextWindowSizeCond) != 0;
        SetWindowSize(window, g.SetNextWindowSizeVal, g.SetNextWindowSizeCond);
        g.SetNextWindowSizeCond = 0;
    }
    if (g.SetNextWindowContentSizeCond) {
        window->SizeContentsExplicit   = g.SetNextWindowContentSizeVal;
        g.SetNextWindowContentSizeCond = 0;
    }
    else if (first_begin_of_the_frame)
    {
        window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    }
    if (g.SetNextWindowCollapsedCond) {
        if (!window_was_active || window_appearing_after_being_hidden)
            window->SetWindowCollapsedAllowFlags |= ImGuiSetCond_Appearing;
        SetWindowCollapsed(window, g.SetNextWindowCollapsedVal, g.SetNextWindowCollapsedCond);
        g.SetNextWindowCollapsedCond = 0;
    }
    if (g.SetNextWindowFocus) {
        ImGui::SetWindowFocus();
        g.SetNextWindowFocus = false;
    }

    // Update known root window (if we are a child window, otherwise window ==
    // window->RootWindow)
    int root_idx, root_non_popup_idx;
    for (root_idx = g.CurrentWindowStack.Size - 1; root_idx > 0; root_idx--)
        if (!(g.CurrentWindowStack[root_idx]->Flags & ImGuiWindowFlags_ChildWindow)) break;
    for (root_non_popup_idx = root_idx; root_non_popup_idx > 0; root_non_popup_idx--)
        if (!(g.CurrentWindowStack[root_non_popup_idx]->Flags &
                (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)))
            break;
    window->ParentWindow       = parent_window;
    window->RootWindow         = g.CurrentWindowStack[root_idx];
    window->RootNonPopupWindow = g.CurrentWindowStack[root_non_popup_idx];  // This is merely for displaying
                                                                            // the TitleBgActive color.

    // When reusing window again multiple times a frame, just append content
    // (don't need to setup again)
    if (first_begin_of_the_frame) {
        window->Active            = true;
        window->IndexWithinParent = 0;
        window->BeginCount        = 0;
        window->ClipRect          = ImVec4(-FLT_MAX, -FLT_MAX, +FLT_MAX, +FLT_MAX);
        window->LastFrameActive   = current_frame;
        window->IDStack.resize(1);

        // Clear draw list, setup texture, outer clipping rectangle
        window->DrawList->Clear();
        window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
        ImRect fullscreen_rect(GetVisibleRect());
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup)))
            PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
        else
            PushClipRect(fullscreen_rect.Min, fullscreen_rect.Max, true);

        if (!window_was_active) {
            // Popup first latch mouse position, will position itself when it appears
            // next frame
            window->AutoPosLastDirection                                                          = -1;
            if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api) window->PosFloat = g.IO.MousePos;
        }

        // Collapse window by double-clicking on title bar
        // At this point we don't have a clipping rectangle setup yet, so we can use
        // the title bar area for hit detection and drawing
        if (!(flags & ImGuiWindowFlags_NoTitleBar) && !(flags & ImGuiWindowFlags_NoCollapse)) {
            ImRect title_bar_rect = window->TitleBarRect();
            if (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) &&
                g.IO.MouseDoubleClicked[0])
            {
                window->Collapsed = !window->Collapsed;
                if (!(flags & ImGuiWindowFlags_NoSavedSettings)) MarkIniSettingsDirty();
                FocusWindow(window);
            }
        }
        else
        {
            window->Collapsed = false;
        }

        // SIZE

        // Save contents size from last frame for auto-fitting (unless explicitly
        // specified)
        window->SizeContents.x =
            (float)(int)((window->SizeContentsExplicit.x != 0.0f)
                             ? window->SizeContentsExplicit.x
                             : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.x - window->Pos.x) + window->Scroll.x));
        window->SizeContents.y =
            (float)(int)((window->SizeContentsExplicit.y != 0.0f)
                             ? window->SizeContentsExplicit.y
                             : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.y - window->Pos.y) + window->Scroll.y));

        // Hide popup/tooltip window when first appearing while we measure size
        // (because we recycle them)
        if (window->HiddenFrames > 0) window->HiddenFrames--;
        if ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0 && !window_was_active) {
            window->HiddenFrames = 1;
            if (flags & ImGuiWindowFlags_AlwaysAutoResize) {
                if (!window_size_set_by_api) window->Size = window->SizeFull = ImVec2(0.f, 0.f);
                window->SizeContents                                         = ImVec2(0.f, 0.f);
            }
        }

        // Lock window padding so that altering the ShowBorders flag for children
        // doesn't have side-effects.
        window->WindowPadding = ((flags & ImGuiWindowFlags_ChildWindow) &&
                                    !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_ShowBorders |
                                                  ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup)))
                                    ? ImVec2(0, 0)
                                    : style.WindowPadding;

        // Calculate auto-fit size
        ImVec2 size_auto_fit;
        if ((flags & ImGuiWindowFlags_Tooltip) != 0) {
            // Tooltip always resize. We keep the spacing symmetric on both axises for
            // aesthetic purpose.
            size_auto_fit = window->SizeContents + window->WindowPadding - ImVec2(0.0f, style.ItemSpacing.y);
        }
        else
        {
            size_auto_fit = ImClamp(window->SizeContents + window->WindowPadding,
                style.WindowMinSize,
                ImMax(style.WindowMinSize, g.IO.DisplaySize - g.Style.DisplaySafeAreaPadding));

            // Handling case of auto fit window not fitting in screen on one axis, we
            // are growing auto fit size on the other axis to compensate for expected
            // scrollbar. FIXME: Might turn bigger than DisplaySize-WindowPadding.
            if (size_auto_fit.x < window->SizeContents.x && !(flags & ImGuiWindowFlags_NoScrollbar) &&
                (flags & ImGuiWindowFlags_HorizontalScrollbar))
                size_auto_fit.y += style.ScrollbarSize;
            if (size_auto_fit.y < window->SizeContents.y && !(flags & ImGuiWindowFlags_NoScrollbar))
                size_auto_fit.x += style.ScrollbarSize;
            size_auto_fit.y = ImMax(size_auto_fit.y - style.ItemSpacing.y, 0.0f);
        }

        // Handle automatic resize
        if (window->Collapsed) {
            // We still process initial auto-fit on collapsed windows to get a window
            // width,
            // But otherwise we don't honor ImGuiWindowFlags_AlwaysAutoResize when
            // collapsed.
            if (window->AutoFitFramesX > 0)
                window->SizeFull.x =
                    window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
            if (window->AutoFitFramesY > 0)
                window->SizeFull.y =
                    window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
        }
        else
        {
            if ((flags & ImGuiWindowFlags_AlwaysAutoResize) && !window_size_set_by_api) {
                window->SizeFull = size_auto_fit;
            }
            else if ((window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0) && !window_size_set_by_api)
            {
                // Auto-fit only grows during the first few frames
                if (window->AutoFitFramesX > 0)
                    window->SizeFull.x =
                        window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
                if (window->AutoFitFramesY > 0)
                    window->SizeFull.y =
                        window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
                if (!(flags & ImGuiWindowFlags_NoSavedSettings)) MarkIniSettingsDirty();
            }
        }

        // Apply minimum/maximum window size constraints and final size
        ApplySizeFullWithConstraint(window, window->SizeFull);
        window->Size = window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;

        // POSITION

        // Position child window
        if (flags & ImGuiWindowFlags_ChildWindow) {
            window->IndexWithinParent = parent_window->DC.ChildWindows.Size;
            parent_window->DC.ChildWindows.push_back(window);
        }
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup)) {
            window->Pos = window->PosFloat = parent_window->DC.CursorPos;
            window->Size = window->SizeFull = size_on_first_use;  // NB: argument name 'size_on_first_use' misleading
                                                                  // here, it's really just 'size' as provided by
                                                                  // user passed via BeginChild()->Begin().
        }

        bool window_pos_center = false;
        window_pos_center |= (window->SetWindowPosCenterWanted && window->HiddenFrames == 0);
        window_pos_center |=
            ((flags & ImGuiWindowFlags_Modal) && !window_pos_set_by_api && window_appearing_after_being_hidden);
        if (window_pos_center) {
            // Center (any sort of window)
            SetWindowPos(
                window, ImMax(style.DisplaySafeAreaPadding, fullscreen_rect.GetCenter() - window->SizeFull * 0.5f), 0);
        }
        else if (flags & ImGuiWindowFlags_ChildMenu)
        {
            // Child menus typically request _any_ position within the parent menu
            // item, and then our FindBestPopupWindowPos() function will move the new
            // menu outside the parent bounds.
            // This is how we end up with child menus appearing (most-commonly) on the
            // right of the parent menu.
            IM_ASSERT(window_pos_set_by_api);
            float horizontal_overlap = style.ItemSpacing.x;  // We want some overlap to convey the relative
                                                             // depth of each popup (currently the amount of
                                                             // overlap it is hard-coded to
                                                             // style.ItemSpacing.x, may need to introduce
                                                             // another style value).
            ImRect rect_to_avoid;
            if (parent_window->DC.MenuBarAppending)
                rect_to_avoid = ImRect(-FLT_MAX,
                    parent_window->Pos.y + parent_window->TitleBarHeight(),
                    FLT_MAX,
                    parent_window->Pos.y + parent_window->TitleBarHeight() + parent_window->MenuBarHeight());
            else
                rect_to_avoid = ImRect(parent_window->Pos.x + horizontal_overlap,
                    -FLT_MAX,
                    parent_window->Pos.x + parent_window->Size.x - horizontal_overlap - parent_window->ScrollbarSizes.x,
                    FLT_MAX);
            window->PosFloat =
                FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
        }
        else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_appearing_after_being_hidden)
        {
            ImRect rect_to_avoid(
                window->PosFloat.x - 1, window->PosFloat.y - 1, window->PosFloat.x + 1, window->PosFloat.y + 1);
            window->PosFloat =
                FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
        }

        // Position tooltip (always follows mouse)
        if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api) {
            ImRect rect_to_avoid(g.IO.MousePos.x - 16,
                g.IO.MousePos.y - 8,
                g.IO.MousePos.x + 24,
                g.IO.MousePos.y + 24);  // FIXME: Completely hard-coded. Perhaps center
                                        // on cursor hit-point instead?
            window->PosFloat =
                FindBestPopupWindowPos(g.IO.MousePos, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
            if (window->AutoPosLastDirection == -1)
                window->PosFloat = g.IO.MousePos + ImVec2(2, 2);  // If there's not enough room, for
                                                                  // tooltip we prefer avoiding the
                                                                  // cursor at all cost even if it means
                                                                  // that part of the tooltip won't be
                                                                  // visible.
        }

        // Clamp position so it stays visible
        if (!(flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Tooltip)) {
            if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 &&
                g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f)  // Ignore zero-sized display explicitly to
                                                                         // avoid losing positions if a window
                                                                         // manager reports zero-sized window when
                                                                         // initializing or minimizing.
            {
                ImVec2 padding   = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
                window->PosFloat = ImMax(window->PosFloat + window->Size, padding) - window->Size;
                window->PosFloat = ImMin(window->PosFloat, g.IO.DisplaySize - padding);
            }
        }
        window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

        // Default item width. Make it proportional to window size if window
        // manually resizes
        if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) &&
            !(flags & ImGuiWindowFlags_AlwaysAutoResize))
            window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
        else
            window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);

        // Prepare for focus requests
        window->FocusIdxAllRequestCurrent =
            (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1)
                ? INT_MAX
                : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter + 1)) %
                      (window->FocusIdxAllCounter + 1);
        window->FocusIdxTabRequestCurrent =
            (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1)
                ? INT_MAX
                : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter + 1)) %
                      (window->FocusIdxTabCounter + 1);
        window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
        window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;

        // Apply scrolling
        if (window->ScrollTarget.x < FLT_MAX) {
            window->Scroll.x       = window->ScrollTarget.x;
            window->ScrollTarget.x = FLT_MAX;
        }
        if (window->ScrollTarget.y < FLT_MAX) {
            float center_ratio = window->ScrollTargetCenterRatio.y;
            window->Scroll.y   = window->ScrollTarget.y -
                               ((1.0f - center_ratio) * (window->TitleBarHeight() + window->MenuBarHeight())) -
                               (center_ratio * window->SizeFull.y);
            window->ScrollTarget.y = FLT_MAX;
        }
        window->Scroll = ImMax(window->Scroll, ImVec2(0.0f, 0.0f));
        if (!window->Collapsed && !window->SkipItems)
            window->Scroll = ImMin(window->Scroll,
                ImMax(ImVec2(0.0f, 0.0f), window->SizeContents - window->SizeFull + window->ScrollbarSizes));

        // Modal window darkens what is behind them
        if ((flags & ImGuiWindowFlags_Modal) != 0 && window == GetFrontMostModalRootWindow())
            window->DrawList->AddRectFilled(fullscreen_rect.Min,
                fullscreen_rect.Max,
                GetColorU32(ImGuiCol_ModalWindowDarkening, g.ModalWindowDarkeningRatio));

        // Draw window + handle manual resize
        ImRect      title_bar_rect = window->TitleBarRect();
        const float window_rounding =
            (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
        if (window->Collapsed) {
            // Draw title bar only
            RenderFrame(title_bar_rect.GetTL(),
                title_bar_rect.GetBR(),
                GetColorU32(ImGuiCol_TitleBgCollapsed),
                true,
                window_rounding);
        }
        else
        {
            ImU32       resize_col         = 0;
            const float resize_corner_size = ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
            if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 &&
                window->AutoFitFramesY <= 0 && !(flags & ImGuiWindowFlags_NoResize))
            {
                // Manual resize
                const ImVec2  br = window->Rect().GetBR();
                const ImRect  resize_rect(br - ImVec2(resize_corner_size * 0.75f, resize_corner_size * 0.75f), br);
                const ImGuiID resize_id = window->GetID("#RESIZE");
                bool          hovered, held;
                ButtonBehavior(resize_rect, resize_id, &hovered, &held, ImGuiButtonFlags_FlattenChilds);
                resize_col = GetColorU32(
                    held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);

                if (hovered || held) g.MouseCursor = ImGuiMouseCursor_ResizeNWSE;

                if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0]) {
                    // Manual auto-fit when double-clicking
                    ApplySizeFullWithConstraint(window, size_auto_fit);
                    if (!(flags & ImGuiWindowFlags_NoSavedSettings)) MarkIniSettingsDirty();
                    ClearActiveID();
                }
                else if (held)
                {
                    // We don't use an incremental MouseDelta but rather compute an
                    // absolute target size based on mouse position
                    ApplySizeFullWithConstraint(
                        window, (g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize()) - window->Pos);
                    if (!(flags & ImGuiWindowFlags_NoSavedSettings)) MarkIniSettingsDirty();
                }

                window->Size   = window->SizeFull;
                title_bar_rect = window->TitleBarRect();
            }

            // Scrollbars
            window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) ||
                                 ((window->SizeContents.y > window->Size.y + style.ItemSpacing.y) &&
                                     !(flags & ImGuiWindowFlags_NoScrollbar));
            window->ScrollbarX =
                (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) ||
                ((window->SizeContents.x >
                     window->Size.x - (window->ScrollbarY ? style.ScrollbarSize : 0.0f) - window->WindowPadding.x) &&
                    !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
            window->ScrollbarSizes = ImVec2(
                window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
            window->BorderSize = (flags & ImGuiWindowFlags_ShowBorders) ? 1.0f : 0.0f;

            // Window background, Default Alpha
            ImGuiCol bg_color_idx = ImGuiCol_WindowBg;
            if ((flags & ImGuiWindowFlags_ComboBox) != 0)
                bg_color_idx = ImGuiCol_ComboBg;
            else if ((flags & ImGuiWindowFlags_Tooltip) != 0 || (flags & ImGuiWindowFlags_Popup) != 0)
                bg_color_idx = ImGuiCol_PopupBg;
            else if ((flags & ImGuiWindowFlags_ChildWindow) != 0)
                bg_color_idx                 = ImGuiCol_ChildWindowBg;
            ImVec4 bg_color                  = style.Colors[bg_color_idx];
            if (bg_alpha >= 0.0f) bg_color.w = bg_alpha;
            bg_color.w *= style.Alpha;
            if (bg_color.w > 0.0f)
                window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()),
                    window->Pos + window->Size,
                    ColorConvertFloat4ToU32(bg_color),
                    window_rounding,
                    (flags & ImGuiWindowFlags_NoTitleBar) ? ImGuiCorner_All
                                                          : ImGuiCorner_BottomLeft | ImGuiCorner_BottomRight);

            // Title bar
            if (!(flags & ImGuiWindowFlags_NoTitleBar))
                window->DrawList->AddRectFilled(title_bar_rect.GetTL(),
                    title_bar_rect.GetBR(),
                    GetColorU32((g.FocusedWindow && window->RootNonPopupWindow == g.FocusedWindow->RootNonPopupWindow)
                                                        ? ImGuiCol_TitleBgActive
                                                        : ImGuiCol_TitleBg),
                    window_rounding,
                    ImGuiCorner_TopLeft | ImGuiCorner_TopRight);

            // Menu bar
            if (flags & ImGuiWindowFlags_MenuBar) {
                ImRect menu_bar_rect = window->MenuBarRect();
                if (flags & ImGuiWindowFlags_ShowBorders)
                    window->DrawList->AddLine(
                        menu_bar_rect.GetBL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border));
                window->DrawList->AddRectFilled(menu_bar_rect.GetTL(),
                    menu_bar_rect.GetBR(),
                    GetColorU32(ImGuiCol_MenuBarBg),
                    (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f,
                    ImGuiCorner_TopLeft | ImGuiCorner_TopRight);
            }

            // Scrollbars
            if (window->ScrollbarX) Scrollbar(window, true);
            if (window->ScrollbarY) Scrollbar(window, false);

            // Render resize grip
            // (after the input handling so we don't have a frame of latency)
            if (!(flags & ImGuiWindowFlags_NoResize)) {
                const ImVec2 br = window->Rect().GetBR();
                window->DrawList->PathLineTo(br + ImVec2(-resize_corner_size, -window->BorderSize));
                window->DrawList->PathLineTo(br + ImVec2(-window->BorderSize, -resize_corner_size));
                window->DrawList->PathArcToFast(
                    ImVec2(br.x - window_rounding - window->BorderSize, br.y - window_rounding - window->BorderSize),
                    window_rounding,
                    0,
                    3);
                window->DrawList->PathFillConvex(resize_col);
            }

            // Borders
            if (flags & ImGuiWindowFlags_ShowBorders) {
                window->DrawList->AddRect(window->Pos + ImVec2(1, 1),
                    window->Pos + window->Size + ImVec2(1, 1),
                    GetColorU32(ImGuiCol_BorderShadow),
                    window_rounding);
                window->DrawList->AddRect(
                    window->Pos, window->Pos + window->Size, GetColorU32(ImGuiCol_Border), window_rounding);
                if (!(flags & ImGuiWindowFlags_NoTitleBar))
                    window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(1, 0),
                        title_bar_rect.GetBR() - ImVec2(1, 0),
                        GetColorU32(ImGuiCol_Border));
            }
        }

        // Update ContentsRegionMax. All the variable it depends on are set above in
        // this function.
        window->ContentsRegionRect.Min.x = -window->Scroll.x + window->WindowPadding.x;
        window->ContentsRegionRect.Min.y =
            -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
        window->ContentsRegionRect.Max.x =
            -window->Scroll.x - window->WindowPadding.x + (window->SizeContentsExplicit.x != 0.0f
                                                                  ? window->SizeContentsExplicit.x
                                                                  : (window->Size.x - window->ScrollbarSizes.x));
        window->ContentsRegionRect.Max.y =
            -window->Scroll.y - window->WindowPadding.y + (window->SizeContentsExplicit.y != 0.0f
                                                                  ? window->SizeContentsExplicit.y
                                                                  : (window->Size.y - window->ScrollbarSizes.y));

        // Setup drawing context
        window->DC.IndentX        = 0.0f + window->WindowPadding.x - window->Scroll.x;
        window->DC.GroupOffsetX   = 0.0f;
        window->DC.ColumnsOffsetX = 0.0f;
        window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.IndentX + window->DC.ColumnsOffsetX,
                                                      window->TitleBarHeight() + window->MenuBarHeight() +
                                                          window->WindowPadding.y - window->Scroll.y);
        window->DC.CursorPos         = window->DC.CursorStartPos;
        window->DC.CursorPosPrevLine = window->DC.CursorPos;
        window->DC.CursorMaxPos      = window->DC.CursorStartPos;
        window->DC.CurrentLineHeight = window->DC.PrevLineHeight = 0.0f;
        window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;
        window->DC.MenuBarAppending                                              = false;
        window->DC.MenuBarOffsetX = ImMax(window->WindowPadding.x, style.ItemSpacing.x);
        window->DC.LogLinePosY    = window->DC.CursorPos.y - 9999.0f;
        window->DC.ChildWindows.resize(0);
        window->DC.LayoutType         = ImGuiLayoutType_Vertical;
        window->DC.ItemWidth          = window->ItemWidthDefault;
        window->DC.TextWrapPos        = -1.0f;  // disabled
        window->DC.AllowKeyboardFocus = true;
        window->DC.ButtonRepeat       = false;
        window->DC.ItemWidthStack.resize(0);
        window->DC.AllowKeyboardFocusStack.resize(0);
        window->DC.ButtonRepeatStack.resize(0);
        window->DC.TextWrapPosStack.resize(0);
        window->DC.ColumnsCurrent   = 0;
        window->DC.ColumnsCount     = 1;
        window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
        window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.ColumnsStartPosY;
        window->DC.TreeDepth                                    = 0;
        window->DC.StateStorage                                 = &window->StateStorage;
        window->DC.GroupStack.resize(0);
        window->DC.ColorEditMode = ImGuiColorEditMode_UserSelect;
        window->MenuColumns.Update(3, style.ItemSpacing.x, !window_was_active);

        if (window->AutoFitFramesX > 0) window->AutoFitFramesX--;
        if (window->AutoFitFramesY > 0) window->AutoFitFramesY--;

        // New windows appears in front (we need to do that AFTER setting
        // DC.CursorStartPos so our initial navigation reference rectangle can start
        // around there)
        if (!window_was_active && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
            if (!(flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Tooltip)) ||
                (flags & ImGuiWindowFlags_Popup))
                FocusWindow(window);

        // Title bar
        if (!(flags & ImGuiWindowFlags_NoTitleBar)) {
            if (p_open != NULL) {
                const float pad = 2.0f;
                const float rad = (window->TitleBarHeight() - pad * 2.0f) * 0.5f;
                if (CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad), rad))
                    *p_open = false;
            }
=======
    if (!window_was_active) {
      // Popup first latch mouse position, will position itself when it appears
      // next frame
      window->AutoPosLastDirection = -1;
      if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
        window->PosFloat = g.IO.MousePos;
    }

    // Collapse window by double-clicking on title bar
    // At this point we don't have a clipping rectangle setup yet, so we can use
    // the title bar area for hit detection and drawing
    if (!(flags & ImGuiWindowFlags_NoTitleBar) &&
        !(flags & ImGuiWindowFlags_NoCollapse)) {
      ImRect title_bar_rect = window->TitleBarRect();
      if (g.HoveredWindow == window &&
          IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) &&
          g.IO.MouseDoubleClicked[0]) {
        window->Collapsed = !window->Collapsed;
        if (!(flags & ImGuiWindowFlags_NoSavedSettings))
          MarkIniSettingsDirty();
        FocusWindow(window);
      }
    } else {
      window->Collapsed = false;
    }

    // SIZE

    // Save contents size from last frame for auto-fitting (unless explicitly
    // specified)
    window->SizeContents.x =
        (float)(int)((window->SizeContentsExplicit.x != 0.0f)
                         ? window->SizeContentsExplicit.x
                         : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.x -
                                                        window->Pos.x) +
                            window->Scroll.x));
    window->SizeContents.y =
        (float)(int)((window->SizeContentsExplicit.y != 0.0f)
                         ? window->SizeContentsExplicit.y
                         : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.y -
                                                        window->Pos.y) +
                            window->Scroll.y));

    // Hide popup/tooltip window when first appearing while we measure size
    // (because we recycle them)
    if (window->HiddenFrames > 0)
      window->HiddenFrames--;
    if ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0 &&
        !window_was_active) {
      window->HiddenFrames = 1;
      if (flags & ImGuiWindowFlags_AlwaysAutoResize) {
        if (!window_size_set_by_api)
          window->Size = window->SizeFull = ImVec2(0.f, 0.f);
        window->SizeContents = ImVec2(0.f, 0.f);
      }
    }

    // Lock window padding so that altering the ShowBorders flag for children
    // doesn't have side-effects.
    window->WindowPadding =
        ((flags & ImGuiWindowFlags_ChildWindow) &&
         !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding |
                    ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_ComboBox |
                    ImGuiWindowFlags_Popup)))
            ? ImVec2(0, 0)
            : style.WindowPadding;

    // Calculate auto-fit size
    ImVec2 size_auto_fit;
    if ((flags & ImGuiWindowFlags_Tooltip) != 0) {
      // Tooltip always resize. We keep the spacing symmetric on both axises for
      // aesthetic purpose.
      size_auto_fit = window->SizeContents + window->WindowPadding -
                      ImVec2(0.0f, style.ItemSpacing.y);
    } else {
      size_auto_fit = ImClamp(
          window->SizeContents + window->WindowPadding, style.WindowMinSize,
          ImMax(style.WindowMinSize,
                g.IO.DisplaySize - g.Style.DisplaySafeAreaPadding));

      // Handling case of auto fit window not fitting in screen on one axis, we
      // are growing auto fit size on the other axis to compensate for expected
      // scrollbar. FIXME: Might turn bigger than DisplaySize-WindowPadding.
      if (size_auto_fit.x < window->SizeContents.x &&
          !(flags & ImGuiWindowFlags_NoScrollbar) &&
          (flags & ImGuiWindowFlags_HorizontalScrollbar))
        size_auto_fit.y += style.ScrollbarSize;
      if (size_auto_fit.y < window->SizeContents.y &&
          !(flags & ImGuiWindowFlags_NoScrollbar))
        size_auto_fit.x += style.ScrollbarSize;
      size_auto_fit.y = ImMax(size_auto_fit.y - style.ItemSpacing.y, 0.0f);
    }

    // Handle automatic resize
    if (window->Collapsed) {
      // We still process initial auto-fit on collapsed windows to get a window
      // width,
      // But otherwise we don't honor ImGuiWindowFlags_AlwaysAutoResize when
      // collapsed.
      if (window->AutoFitFramesX > 0)
        window->SizeFull.x = window->AutoFitOnlyGrows
                                 ? ImMax(window->SizeFull.x, size_auto_fit.x)
                                 : size_auto_fit.x;
      if (window->AutoFitFramesY > 0)
        window->SizeFull.y = window->AutoFitOnlyGrows
                                 ? ImMax(window->SizeFull.y, size_auto_fit.y)
                                 : size_auto_fit.y;
    } else {
      if ((flags & ImGuiWindowFlags_AlwaysAutoResize) &&
          !window_size_set_by_api) {
        window->SizeFull = size_auto_fit;
      } else if ((window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0) &&
                 !window_size_set_by_api) {
        // Auto-fit only grows during the first few frames
        if (window->AutoFitFramesX > 0)
          window->SizeFull.x = window->AutoFitOnlyGrows
                                   ? ImMax(window->SizeFull.x, size_auto_fit.x)
                                   : size_auto_fit.x;
        if (window->AutoFitFramesY > 0)
          window->SizeFull.y = window->AutoFitOnlyGrows
                                   ? ImMax(window->SizeFull.y, size_auto_fit.y)
                                   : size_auto_fit.y;
        if (!(flags & ImGuiWindowFlags_NoSavedSettings))
          MarkIniSettingsDirty();
      }
    }

    // Apply minimum/maximum window size constraints and final size
    ApplySizeFullWithConstraint(window, window->SizeFull);
    window->Size =
        window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;

    // POSITION
>>>>>>> master

    // Position child window
    if (flags & ImGuiWindowFlags_ChildWindow) {
      window->IndexWithinParent = parent_window->DC.ChildWindows.Size;
      parent_window->DC.ChildWindows.push_back(window);
    }
    if ((flags & ImGuiWindowFlags_ChildWindow) &&
        !(flags & ImGuiWindowFlags_Popup)) {
      window->Pos = window->PosFloat = parent_window->DC.CursorPos;
      window->Size = window->SizeFull =
          size_on_first_use; // NB: argument name 'size_on_first_use' misleading
                             // here, it's really just 'size' as provided by
                             // user passed via BeginChild()->Begin().
    }

<<<<<<< HEAD
            ImVec2 text_min = window->Pos;
            ImVec2 text_max = window->Pos + ImVec2(window->Size.x, style.FramePadding.y * 2 + text_size.y);
            ImRect clip_rect;
            clip_rect.Max = ImVec2(
                window->Pos.x + window->Size.x - (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x),
                text_max.y);  // Match the size of CloseWindowButton()
            float pad_left = (flags & ImGuiWindowFlags_NoCollapse) == 0
                                 ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x)
                                 : style.FramePadding.x;
            float pad_right = (p_open != NULL) ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x)
                                               : style.FramePadding.x;
            if (style.WindowTitleAlign.x > 0.0f) pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
            text_min.x += pad_left;
            text_max.x -= pad_right;
            clip_rect.Min = ImVec2(text_min.x, window->Pos.y);
            RenderTextClipped(text_min, text_max, name, NULL, &text_size, style.WindowTitleAlign, &clip_rect);
        }

        // Save clipped aabb so we can access it in constant-time in
        // FindHoveredWindow()
        window->WindowRectClipped = window->Rect();
        window->WindowRectClipped.Clip(window->ClipRect);

        // Pressing CTRL+C while holding on a window copy its content to the
        // clipboard
        // This works but 1. doesn't handle multiple Begin/End pairs, 2. recursing
        // into another Begin/End pair - so we need to work that out and add better
        // logging scope.
        // Maybe we can support CTRL+C on every element?
        /*
        if (g.ActiveId == move_id)
            if (g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_C))
                ImGui::LogToClipboard();
        */
    }

    // Inner clipping rectangle
    // We set this up after processing the resize grip so that our clip rectangle
    // doesn't lag by a frame
    // Note that if our window is collapsed we will end up with a null clipping
    // rectangle which is the correct behavior.
    const ImRect title_bar_rect = window->TitleBarRect();
    const float  border_size    = window->BorderSize;
    ImRect       clip_rect;  // Force round to ensure that e.g. (int)(max.x-min.x) in
                             // user's render code produce correct result.
    clip_rect.Min.x =
        ImFloor(0.5f + title_bar_rect.Min.x + ImMax(border_size, ImFloor(window->WindowPadding.x * 0.5f)));
    clip_rect.Min.y = ImFloor(0.5f + title_bar_rect.Max.y + window->MenuBarHeight() + border_size);
    clip_rect.Max.x = ImFloor(0.5f + window->Pos.x + window->Size.x - window->ScrollbarSizes.x -
                              ImMax(border_size, ImFloor(window->WindowPadding.x * 0.5f)));
    clip_rect.Max.y = ImFloor(0.5f + window->Pos.y + window->Size.y - window->ScrollbarSizes.y - border_size);
    PushClipRect(clip_rect.Min, clip_rect.Max, true);

    // Clear 'accessed' flag last thing
    if (first_begin_of_the_frame) window->Accessed = false;
    window->BeginCount++;
    g.SetNextWindowSizeConstraint = false;

    // Child window can be out of sight and have "negative" clip windows.
    // Mark them as collapsed so commands are skipped earlier (we can't manually
    // collapse because they have no title bar).
    if (flags & ImGuiWindowFlags_ChildWindow) {
        IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);
        window->Collapsed = parent_window && parent_window->Collapsed;

        if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
            window->Collapsed |= (window->WindowRectClipped.Min.x >= window->WindowRectClipped.Max.x ||
                                  window->WindowRectClipped.Min.y >= window->WindowRectClipped.Max.y);

        // We also hide the window from rendering because we've already added its
        // border to the command list.
        // (we could perform the check earlier in the function but it is simpler at
        // this point)
        if (window->Collapsed) window->Active = false;
    }
    if (style.Alpha <= 0.0f) window->Active = false;

    // Return false if we don't intend to display anything to allow user to
    // perform an early out optimization
    window->SkipItems =
        (window->Collapsed || !window->Active) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0;
    return !window->SkipItems;
}

void ImGui::End()
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;

    if (window->DC.ColumnsCount != 1)  // close columns set if any is open
        Columns(1, "#CLOSECOLUMNS");
    PopClipRect();  // inner window clip rectangle

    // Stop logging
    if (!(window->Flags & ImGuiWindowFlags_ChildWindow))  // FIXME: add more
                                                          // options for scope of
                                                          // logging
        LogFinish();

    // Pop
    // NB: we don't clear 'window->RootWindow'. The pointer is allowed to live
    // until the next call to Begin().
    g.CurrentWindowStack.pop_back();
    if (window->Flags & ImGuiWindowFlags_Popup) g.CurrentPopupStack.pop_back();
    CheckStacksSize(window, false);
    SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
=======
    bool window_pos_center = false;
    window_pos_center |=
        (window->SetWindowPosCenterWanted && window->HiddenFrames == 0);
    window_pos_center |=
        ((flags & ImGuiWindowFlags_Modal) && !window_pos_set_by_api &&
         window_appearing_after_being_hidden);
    if (window_pos_center) {
      // Center (any sort of window)
      SetWindowPos(window,
                   ImMax(style.DisplaySafeAreaPadding,
                         fullscreen_rect.GetCenter() - window->SizeFull * 0.5f),
                   0);
    } else if (flags & ImGuiWindowFlags_ChildMenu) {
      // Child menus typically request _any_ position within the parent menu
      // item, and then our FindBestPopupWindowPos() function will move the new
      // menu outside the parent bounds.
      // This is how we end up with child menus appearing (most-commonly) on the
      // right of the parent menu.
      IM_ASSERT(window_pos_set_by_api);
      float horizontal_overlap =
          style.ItemSpacing.x; // We want some overlap to convey the relative
                               // depth of each popup (currently the amount of
                               // overlap it is hard-coded to
                               // style.ItemSpacing.x, may need to introduce
                               // another style value).
      ImRect rect_to_avoid;
      if (parent_window->DC.MenuBarAppending)
        rect_to_avoid = ImRect(
            -FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight(),
            FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight() +
                         parent_window->MenuBarHeight());
      else
        rect_to_avoid =
            ImRect(parent_window->Pos.x + horizontal_overlap, -FLT_MAX,
                   parent_window->Pos.x + parent_window->Size.x -
                       horizontal_overlap - parent_window->ScrollbarSizes.x,
                   FLT_MAX);
      window->PosFloat =
          FindBestPopupWindowPos(window->PosFloat, window->Size,
                                 &window->AutoPosLastDirection, rect_to_avoid);
    } else if ((flags & ImGuiWindowFlags_Popup) != 0 &&
               !window_pos_set_by_api && window_appearing_after_being_hidden) {
      ImRect rect_to_avoid(window->PosFloat.x - 1, window->PosFloat.y - 1,
                           window->PosFloat.x + 1, window->PosFloat.y + 1);
      window->PosFloat =
          FindBestPopupWindowPos(window->PosFloat, window->Size,
                                 &window->AutoPosLastDirection, rect_to_avoid);
    }

    // Position tooltip (always follows mouse)
    if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api) {
      ImRect rect_to_avoid(
          g.IO.MousePos.x - 16, g.IO.MousePos.y - 8, g.IO.MousePos.x + 24,
          g.IO.MousePos.y + 24); // FIXME: Completely hard-coded. Perhaps center
                                 // on cursor hit-point instead?
      window->PosFloat =
          FindBestPopupWindowPos(g.IO.MousePos, window->Size,
                                 &window->AutoPosLastDirection, rect_to_avoid);
      if (window->AutoPosLastDirection == -1)
        window->PosFloat =
            g.IO.MousePos + ImVec2(2, 2); // If there's not enough room, for
                                          // tooltip we prefer avoiding the
                                          // cursor at all cost even if it means
                                          // that part of the tooltip won't be
                                          // visible.
    }

    // Clamp position so it stays visible
    if (!(flags & ImGuiWindowFlags_ChildWindow) &&
        !(flags & ImGuiWindowFlags_Tooltip)) {
      if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 &&
          window->AutoFitFramesY <= 0 && g.IO.DisplaySize.x > 0.0f &&
          g.IO.DisplaySize.y > 0.0f) // Ignore zero-sized display explicitly to
                                     // avoid losing positions if a window
                                     // manager reports zero-sized window when
                                     // initializing or minimizing.
      {
        ImVec2 padding =
            ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
        window->PosFloat =
            ImMax(window->PosFloat + window->Size, padding) - window->Size;
        window->PosFloat = ImMin(window->PosFloat, g.IO.DisplaySize - padding);
      }
    }
    window->Pos =
        ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

    // Default item width. Make it proportional to window size if window
    // manually resizes
    if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) &&
        !(flags & ImGuiWindowFlags_AlwaysAutoResize))
      window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
    else
      window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);

    // Prepare for focus requests
    window->FocusIdxAllRequestCurrent =
        (window->FocusIdxAllRequestNext == INT_MAX ||
         window->FocusIdxAllCounter == -1)
            ? INT_MAX
            : (window->FocusIdxAllRequestNext +
               (window->FocusIdxAllCounter + 1)) %
                  (window->FocusIdxAllCounter + 1);
    window->FocusIdxTabRequestCurrent =
        (window->FocusIdxTabRequestNext == INT_MAX ||
         window->FocusIdxTabCounter == -1)
            ? INT_MAX
            : (window->FocusIdxTabRequestNext +
               (window->FocusIdxTabCounter + 1)) %
                  (window->FocusIdxTabCounter + 1);
    window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
    window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;

    // Apply scrolling
    if (window->ScrollTarget.x < FLT_MAX) {
      window->Scroll.x = window->ScrollTarget.x;
      window->ScrollTarget.x = FLT_MAX;
    }
    if (window->ScrollTarget.y < FLT_MAX) {
      float center_ratio = window->ScrollTargetCenterRatio.y;
      window->Scroll.y = window->ScrollTarget.y -
                         ((1.0f - center_ratio) * (window->TitleBarHeight() +
                                                   window->MenuBarHeight())) -
                         (center_ratio * window->SizeFull.y);
      window->ScrollTarget.y = FLT_MAX;
    }
    window->Scroll = ImMax(window->Scroll, ImVec2(0.0f, 0.0f));
    if (!window->Collapsed && !window->SkipItems)
      window->Scroll =
          ImMin(window->Scroll, ImMax(ImVec2(0.0f, 0.0f),
                                      window->SizeContents - window->SizeFull +
                                          window->ScrollbarSizes));

    // Modal window darkens what is behind them
    if ((flags & ImGuiWindowFlags_Modal) != 0 &&
        window == GetFrontMostModalRootWindow())
      window->DrawList->AddRectFilled(fullscreen_rect.Min, fullscreen_rect.Max,
                                      GetColorU32(ImGuiCol_ModalWindowDarkening,
                                                  g.ModalWindowDarkeningRatio));

    // Draw window + handle manual resize
    ImRect title_bar_rect = window->TitleBarRect();
    const float window_rounding = (flags & ImGuiWindowFlags_ChildWindow)
                                      ? style.ChildWindowRounding
                                      : style.WindowRounding;
    if (window->Collapsed) {
      // Draw title bar only
      RenderFrame(title_bar_rect.GetTL(), title_bar_rect.GetBR(),
                  GetColorU32(ImGuiCol_TitleBgCollapsed), true,
                  window_rounding);
    } else {
      ImU32 resize_col = 0;
      const float resize_corner_size =
          ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
      if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) &&
          window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 &&
          !(flags & ImGuiWindowFlags_NoResize)) {
        // Manual resize
        const ImVec2 br = window->Rect().GetBR();
        const ImRect resize_rect(
            br - ImVec2(resize_corner_size * 0.75f, resize_corner_size * 0.75f),
            br);
        const ImGuiID resize_id = window->GetID("#RESIZE");
        bool hovered, held;
        ButtonBehavior(resize_rect, resize_id, &hovered, &held,
                       ImGuiButtonFlags_FlattenChilds);
        resize_col = GetColorU32(held ? ImGuiCol_ResizeGripActive
                                      : hovered ? ImGuiCol_ResizeGripHovered
                                                : ImGuiCol_ResizeGrip);

        if (hovered || held)
          g.MouseCursor = ImGuiMouseCursor_ResizeNWSE;

        if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0]) {
          // Manual auto-fit when double-clicking
          ApplySizeFullWithConstraint(window, size_auto_fit);
          if (!(flags & ImGuiWindowFlags_NoSavedSettings))
            MarkIniSettingsDirty();
          ClearActiveID();
        } else if (held) {
          // We don't use an incremental MouseDelta but rather compute an
          // absolute target size based on mouse position
          ApplySizeFullWithConstraint(
              window,
              (g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize()) -
                  window->Pos);
          if (!(flags & ImGuiWindowFlags_NoSavedSettings))
            MarkIniSettingsDirty();
        }

        window->Size = window->SizeFull;
        title_bar_rect = window->TitleBarRect();
      }

      // Scrollbars
      window->ScrollbarY =
          (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) ||
          ((window->SizeContents.y > window->Size.y + style.ItemSpacing.y) &&
           !(flags & ImGuiWindowFlags_NoScrollbar));
      window->ScrollbarX =
          (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) ||
          ((window->SizeContents.x >
            window->Size.x - (window->ScrollbarY ? style.ScrollbarSize : 0.0f) -
                window->WindowPadding.x) &&
           !(flags & ImGuiWindowFlags_NoScrollbar) &&
           (flags & ImGuiWindowFlags_HorizontalScrollbar));
      window->ScrollbarSizes =
          ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f,
                 window->ScrollbarX ? style.ScrollbarSize : 0.0f);
      window->BorderSize = (flags & ImGuiWindowFlags_ShowBorders) ? 1.0f : 0.0f;

      // Window background, Default Alpha
      ImGuiCol bg_color_idx = ImGuiCol_WindowBg;
      if ((flags & ImGuiWindowFlags_ComboBox) != 0)
        bg_color_idx = ImGuiCol_ComboBg;
      else if ((flags & ImGuiWindowFlags_Tooltip) != 0 ||
               (flags & ImGuiWindowFlags_Popup) != 0)
        bg_color_idx = ImGuiCol_PopupBg;
      else if ((flags & ImGuiWindowFlags_ChildWindow) != 0)
        bg_color_idx = ImGuiCol_ChildWindowBg;
      ImVec4 bg_color = style.Colors[bg_color_idx];
      if (bg_alpha >= 0.0f)
        bg_color.w = bg_alpha;
      bg_color.w *= style.Alpha;
      if (bg_color.w > 0.0f)
        window->DrawList->AddRectFilled(
            window->Pos + ImVec2(0, window->TitleBarHeight()),
            window->Pos + window->Size, ColorConvertFloat4ToU32(bg_color),
            window_rounding,
            (flags & ImGuiWindowFlags_NoTitleBar)
                ? ImGuiCorner_All
                : ImGuiCorner_BottomLeft | ImGuiCorner_BottomRight);

      // Title bar
      if (!(flags & ImGuiWindowFlags_NoTitleBar))
        window->DrawList->AddRectFilled(
            title_bar_rect.GetTL(), title_bar_rect.GetBR(),
            GetColorU32((g.FocusedWindow &&
                         window->RootNonPopupWindow ==
                             g.FocusedWindow->RootNonPopupWindow)
                            ? ImGuiCol_TitleBgActive
                            : ImGuiCol_TitleBg),
            window_rounding, ImGuiCorner_TopLeft | ImGuiCorner_TopRight);

      // Menu bar
      if (flags & ImGuiWindowFlags_MenuBar) {
        ImRect menu_bar_rect = window->MenuBarRect();
        if (flags & ImGuiWindowFlags_ShowBorders)
          window->DrawList->AddLine(menu_bar_rect.GetBL(),
                                    menu_bar_rect.GetBR(),
                                    GetColorU32(ImGuiCol_Border));
        window->DrawList->AddRectFilled(
            menu_bar_rect.GetTL(), menu_bar_rect.GetBR(),
            GetColorU32(ImGuiCol_MenuBarBg),
            (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f,
            ImGuiCorner_TopLeft | ImGuiCorner_TopRight);
      }

      // Scrollbars
      if (window->ScrollbarX)
        Scrollbar(window, true);
      if (window->ScrollbarY)
        Scrollbar(window, false);

      // Render resize grip
      // (after the input handling so we don't have a frame of latency)
      if (!(flags & ImGuiWindowFlags_NoResize)) {
        const ImVec2 br = window->Rect().GetBR();
        window->DrawList->PathLineTo(
            br + ImVec2(-resize_corner_size, -window->BorderSize));
        window->DrawList->PathLineTo(
            br + ImVec2(-window->BorderSize, -resize_corner_size));
        window->DrawList->PathArcToFast(
            ImVec2(br.x - window_rounding - window->BorderSize,
                   br.y - window_rounding - window->BorderSize),
            window_rounding, 0, 3);
        window->DrawList->PathFillConvex(resize_col);
      }

      // Borders
      if (flags & ImGuiWindowFlags_ShowBorders) {
        window->DrawList->AddRect(window->Pos + ImVec2(1, 1),
                                  window->Pos + window->Size + ImVec2(1, 1),
                                  GetColorU32(ImGuiCol_BorderShadow),
                                  window_rounding);
        window->DrawList->AddRect(window->Pos, window->Pos + window->Size,
                                  GetColorU32(ImGuiCol_Border),
                                  window_rounding);
        if (!(flags & ImGuiWindowFlags_NoTitleBar))
          window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(1, 0),
                                    title_bar_rect.GetBR() - ImVec2(1, 0),
                                    GetColorU32(ImGuiCol_Border));
      }
    }

    // Update ContentsRegionMax. All the variable it depends on are set above in
    // this function.
    window->ContentsRegionRect.Min.x =
        -window->Scroll.x + window->WindowPadding.x;
    window->ContentsRegionRect.Min.y =
        -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() +
        window->MenuBarHeight();
    window->ContentsRegionRect.Max.x =
        -window->Scroll.x - window->WindowPadding.x +
        (window->SizeContentsExplicit.x != 0.0f
             ? window->SizeContentsExplicit.x
             : (window->Size.x - window->ScrollbarSizes.x));
    window->ContentsRegionRect.Max.y =
        -window->Scroll.y - window->WindowPadding.y +
        (window->SizeContentsExplicit.y != 0.0f
             ? window->SizeContentsExplicit.y
             : (window->Size.y - window->ScrollbarSizes.y));

    // Setup drawing context
    window->DC.IndentX = 0.0f + window->WindowPadding.x - window->Scroll.x;
    window->DC.GroupOffsetX = 0.0f;
    window->DC.ColumnsOffsetX = 0.0f;
    window->DC.CursorStartPos =
        window->Pos +
        ImVec2(window->DC.IndentX + window->DC.ColumnsOffsetX,
               window->TitleBarHeight() + window->MenuBarHeight() +
                   window->WindowPadding.y - window->Scroll.y);
    window->DC.CursorPos = window->DC.CursorStartPos;
    window->DC.CursorPosPrevLine = window->DC.CursorPos;
    window->DC.CursorMaxPos = window->DC.CursorStartPos;
    window->DC.CurrentLineHeight = window->DC.PrevLineHeight = 0.0f;
    window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset =
        0.0f;
    window->DC.MenuBarAppending = false;
    window->DC.MenuBarOffsetX =
        ImMax(window->WindowPadding.x, style.ItemSpacing.x);
    window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
    window->DC.ChildWindows.resize(0);
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.ItemWidth = window->ItemWidthDefault;
    window->DC.TextWrapPos = -1.0f; // disabled
    window->DC.AllowKeyboardFocus = true;
    window->DC.ButtonRepeat = false;
    window->DC.ItemWidthStack.resize(0);
    window->DC.AllowKeyboardFocusStack.resize(0);
    window->DC.ButtonRepeatStack.resize(0);
    window->DC.TextWrapPosStack.resize(0);
    window->DC.ColumnsCurrent = 0;
    window->DC.ColumnsCount = 1;
    window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
    window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY =
        window->DC.ColumnsStartPosY;
    window->DC.TreeDepth = 0;
    window->DC.StateStorage = &window->StateStorage;
    window->DC.GroupStack.resize(0);
    window->DC.ColorEditMode = ImGuiColorEditMode_UserSelect;
    window->MenuColumns.Update(3, style.ItemSpacing.x, !window_was_active);

    if (window->AutoFitFramesX > 0)
      window->AutoFitFramesX--;
    if (window->AutoFitFramesY > 0)
      window->AutoFitFramesY--;

    // New windows appears in front (we need to do that AFTER setting
    // DC.CursorStartPos so our initial navigation reference rectangle can start
    // around there)
    if (!window_was_active && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
      if (!(flags &
            (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Tooltip)) ||
          (flags & ImGuiWindowFlags_Popup))
        FocusWindow(window);

    // Title bar
    if (!(flags & ImGuiWindowFlags_NoTitleBar)) {
      if (p_open != NULL) {
        const float pad = 2.0f;
        const float rad = (window->TitleBarHeight() - pad * 2.0f) * 0.5f;
        if (CloseButton(window->GetID("#CLOSE"),
                        window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad),
                        rad))
          *p_open = false;
      }

      const ImVec2 text_size = CalcTextSize(name, NULL, true);
      if (!(flags & ImGuiWindowFlags_NoCollapse))
        RenderCollapseTriangle(window->Pos + style.FramePadding,
                               !window->Collapsed, 1.0f);

      ImVec2 text_min = window->Pos;
      ImVec2 text_max =
          window->Pos +
          ImVec2(window->Size.x, style.FramePadding.y * 2 + text_size.y);
      ImRect clip_rect;
      clip_rect.Max = ImVec2(
          window->Pos.x + window->Size.x -
              (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x),
          text_max.y); // Match the size of CloseWindowButton()
      float pad_left =
          (flags & ImGuiWindowFlags_NoCollapse) == 0
              ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x)
              : style.FramePadding.x;
      float pad_right =
          (p_open != NULL)
              ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x)
              : style.FramePadding.x;
      if (style.WindowTitleAlign.x > 0.0f)
        pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
      text_min.x += pad_left;
      text_max.x -= pad_right;
      clip_rect.Min = ImVec2(text_min.x, window->Pos.y);
      RenderTextClipped(text_min, text_max, name, NULL, &text_size,
                        style.WindowTitleAlign, &clip_rect);
    }

    // Save clipped aabb so we can access it in constant-time in
    // FindHoveredWindow()
    window->WindowRectClipped = window->Rect();
    window->WindowRectClipped.Clip(window->ClipRect);

    // Pressing CTRL+C while holding on a window copy its content to the
    // clipboard
    // This works but 1. doesn't handle multiple Begin/End pairs, 2. recursing
    // into another Begin/End pair - so we need to work that out and add better
    // logging scope.
    // Maybe we can support CTRL+C on every element?
    /*
    if (g.ActiveId == move_id)
        if (g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_C))
            ImGui::LogToClipboard();
    */
  }

  // Inner clipping rectangle
  // We set this up after processing the resize grip so that our clip rectangle
  // doesn't lag by a frame
  // Note that if our window is collapsed we will end up with a null clipping
  // rectangle which is the correct behavior.
  const ImRect title_bar_rect = window->TitleBarRect();
  const float border_size = window->BorderSize;
  ImRect clip_rect; // Force round to ensure that e.g. (int)(max.x-min.x) in
                    // user's render code produce correct result.
  clip_rect.Min.x =
      ImFloor(0.5f + title_bar_rect.Min.x +
              ImMax(border_size, ImFloor(window->WindowPadding.x * 0.5f)));
  clip_rect.Min.y = ImFloor(0.5f + title_bar_rect.Max.y +
                            window->MenuBarHeight() + border_size);
  clip_rect.Max.x =
      ImFloor(0.5f + window->Pos.x + window->Size.x - window->ScrollbarSizes.x -
              ImMax(border_size, ImFloor(window->WindowPadding.x * 0.5f)));
  clip_rect.Max.y = ImFloor(0.5f + window->Pos.y + window->Size.y -
                            window->ScrollbarSizes.y - border_size);
  PushClipRect(clip_rect.Min, clip_rect.Max, true);

  // Clear 'accessed' flag last thing
  if (first_begin_of_the_frame)
    window->Accessed = false;
  window->BeginCount++;
  g.SetNextWindowSizeConstraint = false;

  // Child window can be out of sight and have "negative" clip windows.
  // Mark them as collapsed so commands are skipped earlier (we can't manually
  // collapse because they have no title bar).
  if (flags & ImGuiWindowFlags_ChildWindow) {
    IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);
    window->Collapsed = parent_window && parent_window->Collapsed;

    if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) &&
        window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
      window->Collapsed |=
          (window->WindowRectClipped.Min.x >= window->WindowRectClipped.Max.x ||
           window->WindowRectClipped.Min.y >= window->WindowRectClipped.Max.y);

    // We also hide the window from rendering because we've already added its
    // border to the command list.
    // (we could perform the check earlier in the function but it is simpler at
    // this point)
    if (window->Collapsed)
      window->Active = false;
  }
  if (style.Alpha <= 0.0f)
    window->Active = false;

  // Return false if we don't intend to display anything to allow user to
  // perform an early out optimization
  window->SkipItems = (window->Collapsed || !window->Active) &&
                      window->AutoFitFramesX <= 0 &&
                      window->AutoFitFramesY <= 0;
  return !window->SkipItems;
}

void ImGui::End() {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;

  if (window->DC.ColumnsCount != 1) // close columns set if any is open
    Columns(1, "#CLOSECOLUMNS");
  PopClipRect(); // inner window clip rectangle

  // Stop logging
  if (!(window->Flags & ImGuiWindowFlags_ChildWindow)) // FIXME: add more
                                                       // options for scope of
                                                       // logging
    LogFinish();

  // Pop
  // NB: we don't clear 'window->RootWindow'. The pointer is allowed to live
  // until the next call to Begin().
  g.CurrentWindowStack.pop_back();
  if (window->Flags & ImGuiWindowFlags_Popup)
    g.CurrentPopupStack.pop_back();
  CheckStacksSize(window, false);
  SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL
                                                : g.CurrentWindowStack.back());
>>>>>>> master
}

// Vertical scrollbar
// The entire piece of code below is rather confusing because:
// - We handle absolute seeking (when first clicking outside the grab) and
// relative manipulation (afterward or when clicking inside the grab)
// - We store values as normalized ratio and in a form that allows the window
// content to change while we are holding on a scrollbar
// - We handle both horizontal and vertical scrollbars, which makes the
// terminology not ideal.
<<<<<<< HEAD
static void Scrollbar(ImGuiWindow* window, bool horizontal)
{
    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(horizontal ? "#SCROLLX" : "#SCROLLY");

    // Render background
    bool         other_scrollbar        = (horizontal ? window->ScrollbarY : window->ScrollbarX);
    float        other_scrollbar_size_w = other_scrollbar ? style.ScrollbarSize : 0.0f;
    const ImRect window_rect            = window->Rect();
    const float  border_size            = window->BorderSize;
    ImRect       bb                     = horizontal ? ImRect(window->Pos.x + border_size,
                                 window_rect.Max.y - style.ScrollbarSize,
                                 window_rect.Max.x - other_scrollbar_size_w - border_size,
                                 window_rect.Max.y - border_size)
                           : ImRect(window_rect.Max.x - style.ScrollbarSize,
                                 window->Pos.y + border_size,
                                 window_rect.Max.x - border_size,
                                 window_rect.Max.y - other_scrollbar_size_w - border_size);
    if (!horizontal)
        bb.Min.y +=
            window->TitleBarHeight() + ((window->Flags & ImGuiWindowFlags_MenuBar) ? window->MenuBarHeight() : 0.0f);
    if (bb.GetWidth() <= 0.0f || bb.GetHeight() <= 0.0f) return;

    float window_rounding =
        (window->Flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
    int window_rounding_corners;
=======
static void Scrollbar(ImGuiWindow *window, bool horizontal) {
  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(horizontal ? "#SCROLLX" : "#SCROLLY");

  // Render background
  bool other_scrollbar = (horizontal ? window->ScrollbarY : window->ScrollbarX);
  float other_scrollbar_size_w = other_scrollbar ? style.ScrollbarSize : 0.0f;
  const ImRect window_rect = window->Rect();
  const float border_size = window->BorderSize;
  ImRect bb =
      horizontal
          ? ImRect(window->Pos.x + border_size,
                   window_rect.Max.y - style.ScrollbarSize,
                   window_rect.Max.x - other_scrollbar_size_w - border_size,
                   window_rect.Max.y - border_size)
          : ImRect(window_rect.Max.x - style.ScrollbarSize,
                   window->Pos.y + border_size, window_rect.Max.x - border_size,
                   window_rect.Max.y - other_scrollbar_size_w - border_size);
  if (!horizontal)
    bb.Min.y +=
        window->TitleBarHeight() + ((window->Flags & ImGuiWindowFlags_MenuBar)
                                        ? window->MenuBarHeight()
                                        : 0.0f);
  if (bb.GetWidth() <= 0.0f || bb.GetHeight() <= 0.0f)
    return;

  float window_rounding = (window->Flags & ImGuiWindowFlags_ChildWindow)
                              ? style.ChildWindowRounding
                              : style.WindowRounding;
  int window_rounding_corners;
  if (horizontal)
    window_rounding_corners = ImGuiCorner_BottomLeft |
                              (other_scrollbar ? 0 : ImGuiCorner_BottomRight);
  else
    window_rounding_corners = (((window->Flags & ImGuiWindowFlags_NoTitleBar) &&
                                !(window->Flags & ImGuiWindowFlags_MenuBar))
                                   ? ImGuiCorner_TopRight
                                   : 0) |
                              (other_scrollbar ? 0 : ImGuiCorner_BottomRight);
  window->DrawList->AddRectFilled(bb.Min, bb.Max,
                                  ImGui::GetColorU32(ImGuiCol_ScrollbarBg),
                                  window_rounding, window_rounding_corners);
  bb.Reduce(ImVec2(
      ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f),
      ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));

  // V denote the main axis of the scrollbar
  float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();
  float scroll_v = horizontal ? window->Scroll.x : window->Scroll.y;
  float win_size_avail_v =
      (horizontal ? window->Size.x : window->Size.y) - other_scrollbar_size_w;
  float win_size_contents_v =
      horizontal ? window->SizeContents.x : window->SizeContents.y;

  // The grabable box size generally represent the amount visible (vs the total
  // scrollable amount)
  // But we maintain a minimum size in pixel to allow for the user to still aim
  // inside.
  const float grab_h_pixels =
      ImMin(ImMax(scrollbar_size_v *
                      ImSaturate(win_size_avail_v /
                                 ImMax(win_size_contents_v, win_size_avail_v)),
                  style.GrabMinSize),
            scrollbar_size_v);
  const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

  // Handle input right away. None of the code of Begin() is relying on
  // scrolling position before calling Scrollbar().
  bool held = false;
  bool hovered = false;
  const bool previously_held = (g.ActiveId == id);
  ImGui::ButtonBehavior(bb, id, &hovered, &held);

  float scroll_max = ImMax(1.0f, win_size_contents_v - win_size_avail_v);
  float scroll_ratio = ImSaturate(scroll_v / scroll_max);
  float grab_v_norm =
      scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
  if (held && grab_h_norm < 1.0f) {
    float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
    float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
    float *click_delta_to_grab_center_v =
        horizontal ? &g.ScrollbarClickDeltaToGrabCenter.x
                   : &g.ScrollbarClickDeltaToGrabCenter.y;

    // Click position in scrollbar normalized space (0.0f->1.0f)
    const float clicked_v_norm =
        ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
    ImGui::SetHoveredID(id);

    bool seek_absolute = false;
    if (!previously_held) {
      // On initial click calculate the distance between mouse and the center of
      // the grab
      if (clicked_v_norm >= grab_v_norm &&
          clicked_v_norm <= grab_v_norm + grab_h_norm) {
        *click_delta_to_grab_center_v =
            clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
      } else {
        seek_absolute = true;
        *click_delta_to_grab_center_v = 0.0f;
      }
    }

    // Apply scroll
    // It is ok to modify Scroll here because we are being called in Begin()
    // after the calculation of SizeContents and before setting up our starting
    // position
    const float scroll_v_norm = ImSaturate(
        (clicked_v_norm - *click_delta_to_grab_center_v - grab_h_norm * 0.5f) /
        (1.0f - grab_h_norm));
    scroll_v =
        (float)(int)(0.5f +
                     scroll_v_norm *
                         scroll_max); //(win_size_contents_v - win_size_v));
>>>>>>> master
    if (horizontal)
      window->Scroll.x = scroll_v;
    else
<<<<<<< HEAD
        window_rounding_corners =
            (((window->Flags & ImGuiWindowFlags_NoTitleBar) && !(window->Flags & ImGuiWindowFlags_MenuBar))
                    ? ImGuiCorner_TopRight
                    : 0) |
            (other_scrollbar ? 0 : ImGuiCorner_BottomRight);
    window->DrawList->AddRectFilled(
        bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_ScrollbarBg), window_rounding, window_rounding_corners);
    bb.Reduce(ImVec2(ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f),
        ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));

    // V denote the main axis of the scrollbar
    float scrollbar_size_v    = horizontal ? bb.GetWidth() : bb.GetHeight();
    float scroll_v            = horizontal ? window->Scroll.x : window->Scroll.y;
    float win_size_avail_v    = (horizontal ? window->Size.x : window->Size.y) - other_scrollbar_size_w;
    float win_size_contents_v = horizontal ? window->SizeContents.x : window->SizeContents.y;

    // The grabable box size generally represent the amount visible (vs the total
    // scrollable amount)
    // But we maintain a minimum size in pixel to allow for the user to still aim
    // inside.
    const float grab_h_pixels =
        ImMin(ImMax(scrollbar_size_v * ImSaturate(win_size_avail_v / ImMax(win_size_contents_v, win_size_avail_v)),
                  style.GrabMinSize),
            scrollbar_size_v);
    const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

    // Handle input right away. None of the code of Begin() is relying on
    // scrolling position before calling Scrollbar().
    bool       held            = false;
    bool       hovered         = false;
    const bool previously_held = (g.ActiveId == id);
    ImGui::ButtonBehavior(bb, id, &hovered, &held);

    float scroll_max   = ImMax(1.0f, win_size_contents_v - win_size_avail_v);
    float scroll_ratio = ImSaturate(scroll_v / scroll_max);
    float grab_v_norm  = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
    if (held && grab_h_norm < 1.0f) {
        float  scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
        float  mouse_pos_v     = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
        float* click_delta_to_grab_center_v =
            horizontal ? &g.ScrollbarClickDeltaToGrabCenter.x : &g.ScrollbarClickDeltaToGrabCenter.y;
=======
      window->Scroll.y = scroll_v;

    // Update values for rendering
    scroll_ratio = ImSaturate(scroll_v / scroll_max);
    grab_v_norm =
        scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

    // Update distance to grab now that we have seeked and saturated
    if (seek_absolute)
      *click_delta_to_grab_center_v =
          clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
  }

  // Render
  const ImU32 grab_col = ImGui::GetColorU32(
      held ? ImGuiCol_ScrollbarGrabActive
           : hovered ? ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);
  if (horizontal)
    window->DrawList->AddRectFilled(
        ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y),
        ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels,
               bb.Max.y),
        grab_col, style.ScrollbarRounding);
  else
    window->DrawList->AddRectFilled(
        ImVec2(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm)),
        ImVec2(bb.Max.x,
               ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels),
        grab_col, style.ScrollbarRounding);
}

// Moving window to front of display (which happens to be back of our sorted
// list)
void ImGui::FocusWindow(ImGuiWindow *window) {
  ImGuiContext &g = *GImGui;
>>>>>>> master

  // Always mark the window we passed as focused. This is used for keyboard
  // interactions such as tabbing.
  g.FocusedWindow = window;

<<<<<<< HEAD
        bool seek_absolute = false;
        if (!previously_held) {
            // On initial click calculate the distance between mouse and the center of
            // the grab
            if (clicked_v_norm >= grab_v_norm && clicked_v_norm <= grab_v_norm + grab_h_norm) {
                *click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
            }
            else
            {
                seek_absolute                 = true;
                *click_delta_to_grab_center_v = 0.0f;
            }
        }

        // Apply scroll
        // It is ok to modify Scroll here because we are being called in Begin()
        // after the calculation of SizeContents and before setting up our starting
        // position
        const float scroll_v_norm =
            ImSaturate((clicked_v_norm - *click_delta_to_grab_center_v - grab_h_norm * 0.5f) / (1.0f - grab_h_norm));
        scroll_v = (float)(int)(0.5f + scroll_v_norm * scroll_max);  //(win_size_contents_v - win_size_v));
        if (horizontal)
            window->Scroll.x = scroll_v;
        else
            window->Scroll.y = scroll_v;

        // Update values for rendering
        scroll_ratio = ImSaturate(scroll_v / scroll_max);
        grab_v_norm  = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

        // Update distance to grab now that we have seeked and saturated
        if (seek_absolute) *click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
    }

    // Render
    const ImU32 grab_col = ImGui::GetColorU32(
        held ? ImGuiCol_ScrollbarGrabActive : hovered ? ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);
    if (horizontal)
        window->DrawList->AddRectFilled(ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y),
            ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, bb.Max.y),
            grab_col,
            style.ScrollbarRounding);
    else
        window->DrawList->AddRectFilled(ImVec2(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm)),
            ImVec2(bb.Max.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels),
            grab_col,
            style.ScrollbarRounding);
}

// Moving window to front of display (which happens to be back of our sorted
// list)
void ImGui::FocusWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;

    // Always mark the window we passed as focused. This is used for keyboard
    // interactions such as tabbing.
    g.FocusedWindow = window;

    // Passing NULL allow to disable keyboard focus
    if (!window) return;

    // And move its root window to the top of the pile
    if (window->RootWindow) window = window->RootWindow;

    // Steal focus on active widgets
    if (window->Flags & ImGuiWindowFlags_Popup)  // FIXME: This statement should be
                                                 // unnecessary. Need further
                                                 // testing before removing it..
        if (g.ActiveId != 0 && g.ActiveIdWindow && g.ActiveIdWindow->RootWindow != window) ClearActiveID();

    // Bring to front
    if ((window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus) || g.Windows.back() == window) return;
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i] == window) {
            g.Windows.erase(g.Windows.begin() + i);
            break;
        }
    g.Windows.push_back(window);
}

void ImGui::PushItemWidth(float item_width)
{
    ImGuiWindow* window  = GetCurrentWindow();
    window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault : item_width);
    window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

static void PushMultiItemsWidths(int components, float w_full)
{
    ImGuiWindow*      window   = ImGui::GetCurrentWindow();
    const ImGuiStyle& style    = GImGui->Style;
    if (w_full <= 0.0f) w_full = ImGui::CalcItemWidth();
    const float w_item_one =
        ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
    const float w_item_last =
        ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));
    window->DC.ItemWidthStack.push_back(w_item_last);
    for (int i           = 0; i < components - 1; i++) window->DC.ItemWidthStack.push_back(w_item_one);
    window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemWidthStack.pop_back();
    window->DC.ItemWidth =
        window->DC.ItemWidthStack.empty() ? window->ItemWidthDefault : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    float        w      = window->DC.ItemWidth;
    if (w < 0.0f) {
        // Align to a right-side limit. We include 1 frame padding in the
        // calculation because this is how the width is always used (we add 2 frame
        // padding to it), but we could move that responsibility to the widget as
        // well.
        float width_to_right_edge = GetContentRegionAvail().x;
        w                         = ImMax(1.0f, width_to_right_edge + w);
    }
    w = (float)(int)w;
    return w;
=======
  // Passing NULL allow to disable keyboard focus
  if (!window)
    return;

  // And move its root window to the top of the pile
  if (window->RootWindow)
    window = window->RootWindow;

  // Steal focus on active widgets
  if (window->Flags & ImGuiWindowFlags_Popup) // FIXME: This statement should be
                                              // unnecessary. Need further
                                              // testing before removing it..
    if (g.ActiveId != 0 && g.ActiveIdWindow &&
        g.ActiveIdWindow->RootWindow != window)
      ClearActiveID();

  // Bring to front
  if ((window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus) ||
      g.Windows.back() == window)
    return;
  for (int i = 0; i < g.Windows.Size; i++)
    if (g.Windows[i] == window) {
      g.Windows.erase(g.Windows.begin() + i);
      break;
    }
  g.Windows.push_back(window);
}

void ImGui::PushItemWidth(float item_width) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.ItemWidth =
      (item_width == 0.0f ? window->ItemWidthDefault : item_width);
  window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

static void PushMultiItemsWidths(int components, float w_full) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  const ImGuiStyle &style = GImGui->Style;
  if (w_full <= 0.0f)
    w_full = ImGui::CalcItemWidth();
  const float w_item_one = ImMax(
      1.0f,
      (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components - 1)) /
                   (float)components));
  const float w_item_last =
      ImMax(1.0f, (float)(int)(w_full -
                               (w_item_one + style.ItemInnerSpacing.x) *
                                   (components - 1)));
  window->DC.ItemWidthStack.push_back(w_item_last);
  for (int i = 0; i < components - 1; i++)
    window->DC.ItemWidthStack.push_back(w_item_one);
  window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth() {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.ItemWidthStack.pop_back();
  window->DC.ItemWidth = window->DC.ItemWidthStack.empty()
                             ? window->ItemWidthDefault
                             : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth() {
  ImGuiWindow *window = GetCurrentWindowRead();
  float w = window->DC.ItemWidth;
  if (w < 0.0f) {
    // Align to a right-side limit. We include 1 frame padding in the
    // calculation because this is how the width is always used (we add 2 frame
    // padding to it), but we could move that responsibility to the widget as
    // well.
    float width_to_right_edge = GetContentRegionAvail().x;
    w = ImMax(1.0f, width_to_right_edge + w);
  }
  w = (float)(int)w;
  return w;
}

static ImFont *GetDefaultFont() {
  ImGuiContext &g = *GImGui;
  return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0];
>>>>>>> master
}

static void SetCurrentFont(ImFont *font) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(font && font->IsLoaded()); // Font Atlas not created. Did you call
                                       // io.Fonts->GetTexDataAsRGBA32 /
                                       // GetTexDataAsAlpha8 ?
  IM_ASSERT(font->Scale > 0.0f);
  g.Font = font;
  g.FontBaseSize = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
  g.FontSize = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;
  g.FontTexUvWhitePixel = g.Font->ContainerAtlas->TexUvWhitePixel;
}

<<<<<<< HEAD
static void SetCurrentFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(font && font->IsLoaded());  // Font Atlas not created. Did you call
                                          // io.Fonts->GetTexDataAsRGBA32 /
                                          // GetTexDataAsAlpha8 ?
    IM_ASSERT(font->Scale > 0.0f);
    g.Font                = font;
    g.FontBaseSize        = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
    g.FontSize            = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;
    g.FontTexUvWhitePixel = g.Font->ContainerAtlas->TexUvWhitePixel;
}

void ImGui::PushFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    if (!font) font = GetDefaultFont();
    SetCurrentFont(font);
    g.FontStack.push_back(font);
    g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void ImGui::PopFont()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DrawList->PopTextureID();
    g.FontStack.pop_back();
    SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void ImGui::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
    ImGuiWindow* window           = GetCurrentWindow();
    window->DC.AllowKeyboardFocus = allow_keyboard_focus;
    window->DC.AllowKeyboardFocusStack.push_back(allow_keyboard_focus);
}

void ImGui::PopAllowKeyboardFocus()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.AllowKeyboardFocusStack.pop_back();
    window->DC.AllowKeyboardFocus =
        window->DC.AllowKeyboardFocusStack.empty() ? true : window->DC.AllowKeyboardFocusStack.back();
}

void ImGui::PushButtonRepeat(bool repeat)
{
    ImGuiWindow* window     = GetCurrentWindow();
    window->DC.ButtonRepeat = repeat;
    window->DC.ButtonRepeatStack.push_back(repeat);
=======
void ImGui::PushFont(ImFont *font) {
  ImGuiContext &g = *GImGui;
  if (!font)
    font = GetDefaultFont();
  SetCurrentFont(font);
  g.FontStack.push_back(font);
  g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void ImGui::PopFont() {
  ImGuiContext &g = *GImGui;
  g.CurrentWindow->DrawList->PopTextureID();
  g.FontStack.pop_back();
  SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void ImGui::PushAllowKeyboardFocus(bool allow_keyboard_focus) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.AllowKeyboardFocus = allow_keyboard_focus;
  window->DC.AllowKeyboardFocusStack.push_back(allow_keyboard_focus);
}

void ImGui::PopAllowKeyboardFocus() {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.AllowKeyboardFocusStack.pop_back();
  window->DC.AllowKeyboardFocus =
      window->DC.AllowKeyboardFocusStack.empty()
          ? true
          : window->DC.AllowKeyboardFocusStack.back();
}

void ImGui::PushButtonRepeat(bool repeat) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.ButtonRepeat = repeat;
  window->DC.ButtonRepeatStack.push_back(repeat);
}

void ImGui::PopButtonRepeat() {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.ButtonRepeatStack.pop_back();
  window->DC.ButtonRepeat = window->DC.ButtonRepeatStack.empty()
                                ? false
                                : window->DC.ButtonRepeatStack.back();
>>>>>>> master
}

void ImGui::PushTextWrapPos(float wrap_pos_x) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.TextWrapPos = wrap_pos_x;
  window->DC.TextWrapPosStack.push_back(wrap_pos_x);
}

<<<<<<< HEAD
void ImGui::PushTextWrapPos(float wrap_pos_x)
{
    ImGuiWindow* window    = GetCurrentWindow();
    window->DC.TextWrapPos = wrap_pos_x;
    window->DC.TextWrapPosStack.push_back(wrap_pos_x);
=======
void ImGui::PopTextWrapPos() {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.TextWrapPosStack.pop_back();
  window->DC.TextWrapPos = window->DC.TextWrapPosStack.empty()
                               ? -1.0f
                               : window->DC.TextWrapPosStack.back();
>>>>>>> master
}

void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4 &col) {
  ImGuiContext &g = *GImGui;
  ImGuiColMod backup;
  backup.Col = idx;
  backup.BackupValue = g.Style.Colors[idx];
  g.ColorModifiers.push_back(backup);
  g.Style.Colors[idx] = col;
}

<<<<<<< HEAD
void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColMod   backup;
    backup.Col         = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorModifiers.push_back(backup);
    g.Style.Colors[idx] = col;
}

void ImGui::PopStyleColor(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0) {
        ImGuiColMod& backup        = g.ColorModifiers.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorModifiers.pop_back();
        count--;
    }
}

struct ImGuiStyleVarInfo
{
    ImGuiDataType Type;
    ImU32         Offset;
    void*         GetVarPtr() const { return (void*)((unsigned char*)&GImGui->Style + Offset); }
=======
void ImGui::PopStyleColor(int count) {
  ImGuiContext &g = *GImGui;
  while (count > 0) {
    ImGuiColMod &backup = g.ColorModifiers.back();
    g.Style.Colors[backup.Col] = backup.BackupValue;
    g.ColorModifiers.pop_back();
    count--;
  }
}

struct ImGuiStyleVarInfo {
  ImGuiDataType Type;
  ImU32 Offset;
  void *GetVarPtr() const {
    return (void *)((unsigned char *)&GImGui->Style + Offset);
  }
>>>>>>> master
};

static const ImGuiStyleVarInfo GStyleVarInfo[ImGuiStyleVar_Count_] = {
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, Alpha)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowPadding)},
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowRounding)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowMinSize)},
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildWindowRounding)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, FramePadding)},
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameRounding)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemSpacing)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing)},
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, IndentSpacing)},
    {ImGuiDataType_Float, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabMinSize)},
    {ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ButtonTextAlign)},
};

static const ImGuiStyleVarInfo *GetStyleVarInfo(ImGuiStyleVar idx) {
  IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_Count_);
  return &GStyleVarInfo[idx];
}

<<<<<<< HEAD
void ImGui::PushStyleVar(ImGuiStyleVar idx, float val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float) {
        float* pvar = (float*)var_info->GetVarPtr();
        GImGui->StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0);  // Called function with wrong-type? Variable is not a float.
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float2) {
        ImVec2* pvar = (ImVec2*)var_info->GetVarPtr();
        GImGui->StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0);  // Called function with wrong-type? Variable is not a ImVec2.
}

void ImGui::PopStyleVar(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0) {
        ImGuiStyleMod&           backup = g.StyleModifiers.back();
        const ImGuiStyleVarInfo* info   = GetStyleVarInfo(backup.VarIdx);
        if (info->Type == ImGuiDataType_Float)
            (*(float*)info->GetVarPtr()) = backup.BackupFloat[0];
        else if (info->Type == ImGuiDataType_Float2)
            (*(ImVec2*)info->GetVarPtr()) = ImVec2(backup.BackupFloat[0], backup.BackupFloat[1]);
        else if (info->Type == ImGuiDataType_Int)
            (*(int*)info->GetVarPtr()) = backup.BackupInt[0];
        g.StyleModifiers.pop_back();
        count--;
    }
}

const char* ImGui::GetStyleColName(ImGuiCol idx)
{
    // Create switch-case from enum with regexp: ImGuiCol_{.*}, --> case
    // ImGuiCol_\1: return "\1";
    switch (idx)
    {
        case ImGuiCol_Text: return "Text";
        case ImGuiCol_TextDisabled: return "TextDisabled";
        case ImGuiCol_WindowBg: return "WindowBg";
        case ImGuiCol_ChildWindowBg: return "ChildWindowBg";
        case ImGuiCol_PopupBg: return "PopupBg";
        case ImGuiCol_Border: return "Border";
        case ImGuiCol_BorderShadow: return "BorderShadow";
        case ImGuiCol_FrameBg: return "FrameBg";
        case ImGuiCol_FrameBgHovered: return "FrameBgHovered";
        case ImGuiCol_FrameBgActive: return "FrameBgActive";
        case ImGuiCol_TitleBg: return "TitleBg";
        case ImGuiCol_TitleBgCollapsed: return "TitleBgCollapsed";
        case ImGuiCol_TitleBgActive: return "TitleBgActive";
        case ImGuiCol_MenuBarBg: return "MenuBarBg";
        case ImGuiCol_ScrollbarBg: return "ScrollbarBg";
        case ImGuiCol_ScrollbarGrab: return "ScrollbarGrab";
        case ImGuiCol_ScrollbarGrabHovered: return "ScrollbarGrabHovered";
        case ImGuiCol_ScrollbarGrabActive: return "ScrollbarGrabActive";
        case ImGuiCol_ComboBg: return "ComboBg";
        case ImGuiCol_CheckMark: return "CheckMark";
        case ImGuiCol_SliderGrab: return "SliderGrab";
        case ImGuiCol_SliderGrabActive: return "SliderGrabActive";
        case ImGuiCol_Button: return "Button";
        case ImGuiCol_ButtonHovered: return "ButtonHovered";
        case ImGuiCol_ButtonActive: return "ButtonActive";
        case ImGuiCol_Header: return "Header";
        case ImGuiCol_HeaderHovered: return "HeaderHovered";
        case ImGuiCol_HeaderActive: return "HeaderActive";
        case ImGuiCol_Column: return "Column";
        case ImGuiCol_ColumnHovered: return "ColumnHovered";
        case ImGuiCol_ColumnActive: return "ColumnActive";
        case ImGuiCol_ResizeGrip: return "ResizeGrip";
        case ImGuiCol_ResizeGripHovered: return "ResizeGripHovered";
        case ImGuiCol_ResizeGripActive: return "ResizeGripActive";
        case ImGuiCol_CloseButton: return "CloseButton";
        case ImGuiCol_CloseButtonHovered: return "CloseButtonHovered";
        case ImGuiCol_CloseButtonActive: return "CloseButtonActive";
        case ImGuiCol_PlotLines: return "PlotLines";
        case ImGuiCol_PlotLinesHovered: return "PlotLinesHovered";
        case ImGuiCol_PlotHistogram: return "PlotHistogram";
        case ImGuiCol_PlotHistogramHovered: return "PlotHistogramHovered";
        case ImGuiCol_TextSelectedBg: return "TextSelectedBg";
        case ImGuiCol_ModalWindowDarkening: return "ModalWindowDarkening";
    }
    IM_ASSERT(0);
    return "Unknown";
=======
void ImGui::PushStyleVar(ImGuiStyleVar idx, float val) {
  const ImGuiStyleVarInfo *var_info = GetStyleVarInfo(idx);
  if (var_info->Type == ImGuiDataType_Float) {
    float *pvar = (float *)var_info->GetVarPtr();
    GImGui->StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
    *pvar = val;
    return;
  }
  IM_ASSERT(0); // Called function with wrong-type? Variable is not a float.
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, const ImVec2 &val) {
  const ImGuiStyleVarInfo *var_info = GetStyleVarInfo(idx);
  if (var_info->Type == ImGuiDataType_Float2) {
    ImVec2 *pvar = (ImVec2 *)var_info->GetVarPtr();
    GImGui->StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
    *pvar = val;
    return;
  }
  IM_ASSERT(0); // Called function with wrong-type? Variable is not a ImVec2.
}

void ImGui::PopStyleVar(int count) {
  ImGuiContext &g = *GImGui;
  while (count > 0) {
    ImGuiStyleMod &backup = g.StyleModifiers.back();
    const ImGuiStyleVarInfo *info = GetStyleVarInfo(backup.VarIdx);
    if (info->Type == ImGuiDataType_Float)
      (*(float *)info->GetVarPtr()) = backup.BackupFloat[0];
    else if (info->Type == ImGuiDataType_Float2)
      (*(ImVec2 *)info->GetVarPtr()) =
          ImVec2(backup.BackupFloat[0], backup.BackupFloat[1]);
    else if (info->Type == ImGuiDataType_Int)
      (*(int *)info->GetVarPtr()) = backup.BackupInt[0];
    g.StyleModifiers.pop_back();
    count--;
  }
}

const char *ImGui::GetStyleColName(ImGuiCol idx) {
  // Create switch-case from enum with regexp: ImGuiCol_{.*}, --> case
  // ImGuiCol_\1: return "\1";
  switch (idx) {
  case ImGuiCol_Text:
    return "Text";
  case ImGuiCol_TextDisabled:
    return "TextDisabled";
  case ImGuiCol_WindowBg:
    return "WindowBg";
  case ImGuiCol_ChildWindowBg:
    return "ChildWindowBg";
  case ImGuiCol_PopupBg:
    return "PopupBg";
  case ImGuiCol_Border:
    return "Border";
  case ImGuiCol_BorderShadow:
    return "BorderShadow";
  case ImGuiCol_FrameBg:
    return "FrameBg";
  case ImGuiCol_FrameBgHovered:
    return "FrameBgHovered";
  case ImGuiCol_FrameBgActive:
    return "FrameBgActive";
  case ImGuiCol_TitleBg:
    return "TitleBg";
  case ImGuiCol_TitleBgCollapsed:
    return "TitleBgCollapsed";
  case ImGuiCol_TitleBgActive:
    return "TitleBgActive";
  case ImGuiCol_MenuBarBg:
    return "MenuBarBg";
  case ImGuiCol_ScrollbarBg:
    return "ScrollbarBg";
  case ImGuiCol_ScrollbarGrab:
    return "ScrollbarGrab";
  case ImGuiCol_ScrollbarGrabHovered:
    return "ScrollbarGrabHovered";
  case ImGuiCol_ScrollbarGrabActive:
    return "ScrollbarGrabActive";
  case ImGuiCol_ComboBg:
    return "ComboBg";
  case ImGuiCol_CheckMark:
    return "CheckMark";
  case ImGuiCol_SliderGrab:
    return "SliderGrab";
  case ImGuiCol_SliderGrabActive:
    return "SliderGrabActive";
  case ImGuiCol_Button:
    return "Button";
  case ImGuiCol_ButtonHovered:
    return "ButtonHovered";
  case ImGuiCol_ButtonActive:
    return "ButtonActive";
  case ImGuiCol_Header:
    return "Header";
  case ImGuiCol_HeaderHovered:
    return "HeaderHovered";
  case ImGuiCol_HeaderActive:
    return "HeaderActive";
  case ImGuiCol_Column:
    return "Column";
  case ImGuiCol_ColumnHovered:
    return "ColumnHovered";
  case ImGuiCol_ColumnActive:
    return "ColumnActive";
  case ImGuiCol_ResizeGrip:
    return "ResizeGrip";
  case ImGuiCol_ResizeGripHovered:
    return "ResizeGripHovered";
  case ImGuiCol_ResizeGripActive:
    return "ResizeGripActive";
  case ImGuiCol_CloseButton:
    return "CloseButton";
  case ImGuiCol_CloseButtonHovered:
    return "CloseButtonHovered";
  case ImGuiCol_CloseButtonActive:
    return "CloseButtonActive";
  case ImGuiCol_PlotLines:
    return "PlotLines";
  case ImGuiCol_PlotLinesHovered:
    return "PlotLinesHovered";
  case ImGuiCol_PlotHistogram:
    return "PlotHistogram";
  case ImGuiCol_PlotHistogramHovered:
    return "PlotHistogramHovered";
  case ImGuiCol_TextSelectedBg:
    return "TextSelectedBg";
  case ImGuiCol_ModalWindowDarkening:
    return "ModalWindowDarkening";
  }
  IM_ASSERT(0);
  return "Unknown";
>>>>>>> master
}

bool ImGui::IsWindowHovered() {
  ImGuiContext &g = *GImGui;
  return g.HoveredWindow == g.CurrentWindow &&
         IsWindowContentHoverable(g.HoveredRootWindow);
}

bool ImGui::IsWindowFocused() {
  ImGuiContext &g = *GImGui;
  return g.FocusedWindow == g.CurrentWindow;
}

bool ImGui::IsRootWindowFocused() {
  ImGuiContext &g = *GImGui;
  return g.FocusedWindow == g.CurrentWindow->RootWindow;
}

bool ImGui::IsRootWindowOrAnyChildFocused() {
  ImGuiContext &g = *GImGui;
  return g.FocusedWindow &&
         g.FocusedWindow->RootWindow == g.CurrentWindow->RootWindow;
}

<<<<<<< HEAD
bool ImGui::IsRootWindowOrAnyChildHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredRootWindow && (g.HoveredRootWindow == g.CurrentWindow->RootWindow) &&
           IsWindowContentHoverable(g.HoveredRootWindow);
=======
bool ImGui::IsRootWindowOrAnyChildHovered() {
  ImGuiContext &g = *GImGui;
  return g.HoveredRootWindow &&
         (g.HoveredRootWindow == g.CurrentWindow->RootWindow) &&
         IsWindowContentHoverable(g.HoveredRootWindow);
>>>>>>> master
}

float ImGui::GetWindowWidth() {
  ImGuiWindow *window = GImGui->CurrentWindow;
  return window->Size.x;
}

float ImGui::GetWindowHeight() {
  ImGuiWindow *window = GImGui->CurrentWindow;
  return window->Size.y;
}

<<<<<<< HEAD
ImVec2 ImGui::GetWindowPos()
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;
    return window->Pos;
=======
ImVec2 ImGui::GetWindowPos() {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;
  return window->Pos;
>>>>>>> master
}

static void SetWindowScrollY(ImGuiWindow *window, float new_scroll_y) {
  window->DC.CursorMaxPos.y += window->Scroll.y;
  window->Scroll.y = new_scroll_y;
  window->DC.CursorMaxPos.y -= window->Scroll.y;
}

<<<<<<< HEAD
static void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiSetCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowPosAllowFlags & cond) == 0) return;
    window->SetWindowPosAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);
    window->SetWindowPosCenterWanted = false;

    // Set
    const ImVec2 old_pos = window->Pos;
    window->PosFloat     = pos;
    window->Pos          = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
    window->DC.CursorPos += (window->Pos - old_pos);     // As we happen to move the window while it is
                                                         // being appended to (which is a bad idea - will
                                                         // smear) let's at least offset the cursor
    window->DC.CursorMaxPos += (window->Pos - old_pos);  // And more importantly we need to adjust this so
                                                         // size calculation doesn't get affected.
=======
static void SetWindowPos(ImGuiWindow *window, const ImVec2 &pos,
                         ImGuiSetCond cond) {
  // Test condition (NB: bit 0 is always true) and clear flags for next time
  if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
    return;
  window->SetWindowPosAllowFlags &=
      ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);
  window->SetWindowPosCenterWanted = false;

  // Set
  const ImVec2 old_pos = window->Pos;
  window->PosFloat = pos;
  window->Pos =
      ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
  window->DC.CursorPos +=
      (window->Pos - old_pos); // As we happen to move the window while it is
                               // being appended to (which is a bad idea - will
                               // smear) let's at least offset the cursor
  window->DC.CursorMaxPos +=
      (window->Pos - old_pos); // And more importantly we need to adjust this so
                               // size calculation doesn't get affected.
>>>>>>> master
}

void ImGui::SetWindowPos(const ImVec2 &pos, ImGuiSetCond cond) {
  ImGuiWindow *window = GetCurrentWindowRead();
  SetWindowPos(window, pos, cond);
}

void ImGui::SetWindowPos(const char *name, const ImVec2 &pos,
                         ImGuiSetCond cond) {
  if (ImGuiWindow *window = FindWindowByName(name))
    SetWindowPos(window, pos, cond);
}

<<<<<<< HEAD
void ImGui::SetWindowPos(const char* name, const ImVec2& pos, ImGuiSetCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name)) SetWindowPos(window, pos, cond);
=======
ImVec2 ImGui::GetWindowSize() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->Size;
>>>>>>> master
}

static void SetWindowSize(ImGuiWindow *window, const ImVec2 &size,
                          ImGuiSetCond cond) {
  // Test condition (NB: bit 0 is always true) and clear flags for next time
  if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
    return;
  window->SetWindowSizeAllowFlags &=
      ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

  // Set
  if (size.x > 0.0f) {
    window->AutoFitFramesX = 0;
    window->SizeFull.x = size.x;
  } else {
    window->AutoFitFramesX = 2;
    window->AutoFitOnlyGrows = false;
  }
  if (size.y > 0.0f) {
    window->AutoFitFramesY = 0;
    window->SizeFull.y = size.y;
  } else {
    window->AutoFitFramesY = 2;
    window->AutoFitOnlyGrows = false;
  }
}

<<<<<<< HEAD
static void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiSetCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowSizeAllowFlags & cond) == 0) return;
    window->SetWindowSizeAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

    // Set
    if (size.x > 0.0f) {
        window->AutoFitFramesX = 0;
        window->SizeFull.x     = size.x;
    }
    else
    {
        window->AutoFitFramesX   = 2;
        window->AutoFitOnlyGrows = false;
    }
    if (size.y > 0.0f) {
        window->AutoFitFramesY = 0;
        window->SizeFull.y     = size.y;
    }
    else
    {
        window->AutoFitFramesY   = 2;
        window->AutoFitOnlyGrows = false;
    }
}

void ImGui::SetWindowSize(const ImVec2& size, ImGuiSetCond cond) { SetWindowSize(GImGui->CurrentWindow, size, cond); }

void ImGui::SetWindowSize(const char* name, const ImVec2& size, ImGuiSetCond cond)
{
    ImGuiWindow* window = FindWindowByName(name);
    if (window) SetWindowSize(window, size, cond);
}

static void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiSetCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0) return;
    window->SetWindowCollapsedAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

    // Set
    window->Collapsed = collapsed;
=======
void ImGui::SetWindowSize(const ImVec2 &size, ImGuiSetCond cond) {
  SetWindowSize(GImGui->CurrentWindow, size, cond);
}

void ImGui::SetWindowSize(const char *name, const ImVec2 &size,
                          ImGuiSetCond cond) {
  ImGuiWindow *window = FindWindowByName(name);
  if (window)
    SetWindowSize(window, size, cond);
}

static void SetWindowCollapsed(ImGuiWindow *window, bool collapsed,
                               ImGuiSetCond cond) {
  // Test condition (NB: bit 0 is always true) and clear flags for next time
  if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
    return;
  window->SetWindowCollapsedAllowFlags &=
      ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

  // Set
  window->Collapsed = collapsed;
}

void ImGui::SetWindowCollapsed(bool collapsed, ImGuiSetCond cond) {
  SetWindowCollapsed(GImGui->CurrentWindow, collapsed, cond);
>>>>>>> master
}

bool ImGui::IsWindowCollapsed() { return GImGui->CurrentWindow->Collapsed; }

void ImGui::SetWindowCollapsed(const char *name, bool collapsed,
                               ImGuiSetCond cond) {
  ImGuiWindow *window = FindWindowByName(name);
  if (window)
    SetWindowCollapsed(window, collapsed, cond);
}

<<<<<<< HEAD
bool ImGui::IsWindowCollapsed() { return GImGui->CurrentWindow->Collapsed; }

void ImGui::SetWindowCollapsed(const char* name, bool collapsed, ImGuiSetCond cond)
{
    ImGuiWindow* window = FindWindowByName(name);
    if (window) SetWindowCollapsed(window, collapsed, cond);
}

void ImGui::SetWindowFocus() { FocusWindow(GImGui->CurrentWindow); }

void ImGui::SetWindowFocus(const char* name)
{
    if (name) {
        if (ImGuiWindow* window = FindWindowByName(name)) FocusWindow(window);
    }
    else
    {
        FocusWindow(NULL);
    }
}

void ImGui::SetNextWindowPos(const ImVec2& pos, ImGuiSetCond cond)
{
    ImGuiContext& g        = *GImGui;
    g.SetNextWindowPosVal  = pos;
    g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowPosCenter(ImGuiSetCond cond)
{
    ImGuiContext& g        = *GImGui;
    g.SetNextWindowPosVal  = ImVec2(-FLT_MAX, -FLT_MAX);
    g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiSetCond cond)
{
    ImGuiContext& g         = *GImGui;
    g.SetNextWindowSizeVal  = size;
    g.SetNextWindowSizeCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max,
    ImGuiSizeConstraintCallback custom_callback, void* custom_callback_user_data)
{
    ImGuiContext& g                               = *GImGui;
    g.SetNextWindowSizeConstraint                 = true;
    g.SetNextWindowSizeConstraintRect             = ImRect(size_min, size_max);
    g.SetNextWindowSizeConstraintCallback         = custom_callback;
    g.SetNextWindowSizeConstraintCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2& size)
{
    ImGuiContext& g                = *GImGui;
    g.SetNextWindowContentSizeVal  = size;
    g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowContentWidth(float width)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowContentSizeVal =
        ImVec2(width, g.SetNextWindowContentSizeCond ? g.SetNextWindowContentSizeVal.y : 0.0f);
    g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond)
{
    ImGuiContext& g              = *GImGui;
    g.SetNextWindowCollapsedVal  = collapsed;
    g.SetNextWindowCollapsedCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowFocus()
{
    ImGuiContext& g      = *GImGui;
    g.SetNextWindowFocus = true;
}

// In window space (not screen space!)
ImVec2 ImGui::GetContentRegionMax()
{
    ImGuiWindow* window                    = GetCurrentWindowRead();
    ImVec2       mx                        = window->ContentsRegionRect.Max;
    if (window->DC.ColumnsCount != 1) mx.x = GetColumnOffset(window->DC.ColumnsCurrent + 1) - window->WindowPadding.x;
    return mx;
=======
void ImGui::SetWindowFocus() { FocusWindow(GImGui->CurrentWindow); }

void ImGui::SetWindowFocus(const char *name) {
  if (name) {
    if (ImGuiWindow *window = FindWindowByName(name))
      FocusWindow(window);
  } else {
    FocusWindow(NULL);
  }
}

void ImGui::SetNextWindowPos(const ImVec2 &pos, ImGuiSetCond cond) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowPosVal = pos;
  g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowPosCenter(ImGuiSetCond cond) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowPosVal = ImVec2(-FLT_MAX, -FLT_MAX);
  g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSize(const ImVec2 &size, ImGuiSetCond cond) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowSizeVal = size;
  g.SetNextWindowSizeCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(
    const ImVec2 &size_min, const ImVec2 &size_max,
    ImGuiSizeConstraintCallback custom_callback,
    void *custom_callback_user_data) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowSizeConstraint = true;
  g.SetNextWindowSizeConstraintRect = ImRect(size_min, size_max);
  g.SetNextWindowSizeConstraintCallback = custom_callback;
  g.SetNextWindowSizeConstraintCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2 &size) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowContentSizeVal = size;
  g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowContentWidth(float width) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowContentSizeVal = ImVec2(
      width,
      g.SetNextWindowContentSizeCond ? g.SetNextWindowContentSizeVal.y : 0.0f);
  g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond) {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowCollapsedVal = collapsed;
  g.SetNextWindowCollapsedCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowFocus() {
  ImGuiContext &g = *GImGui;
  g.SetNextWindowFocus = true;
}

// In window space (not screen space!)
ImVec2 ImGui::GetContentRegionMax() {
  ImGuiWindow *window = GetCurrentWindowRead();
  ImVec2 mx = window->ContentsRegionRect.Max;
  if (window->DC.ColumnsCount != 1)
    mx.x = GetColumnOffset(window->DC.ColumnsCurrent + 1) -
           window->WindowPadding.x;
  return mx;
>>>>>>> master
}

ImVec2 ImGui::GetContentRegionAvail() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return GetContentRegionMax() - (window->DC.CursorPos - window->Pos);
}

float ImGui::GetContentRegionAvailWidth() { return GetContentRegionAvail().x; }

// In window space (not screen space!)
ImVec2 ImGui::GetWindowContentRegionMin() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->ContentsRegionRect.Min;
}

ImVec2 ImGui::GetWindowContentRegionMax() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->ContentsRegionRect.Max;
}

float ImGui::GetWindowContentRegionWidth() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->ContentsRegionRect.Max.x - window->ContentsRegionRect.Min.x;
}

float ImGui::GetTextLineHeight() {
  ImGuiContext &g = *GImGui;
  return g.FontSize;
}

float ImGui::GetTextLineHeightWithSpacing() {
  ImGuiContext &g = *GImGui;
  return g.FontSize + g.Style.ItemSpacing.y;
}

float ImGui::GetItemsLineHeightWithSpacing() {
  ImGuiContext &g = *GImGui;
  return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

ImDrawList *ImGui::GetWindowDrawList() {
  ImGuiWindow *window = GetCurrentWindow();
  return window->DrawList;
}

<<<<<<< HEAD
ImFont* ImGui::GetFont() { return GImGui->Font; }

float ImGui::GetFontSize() { return GImGui->FontSize; }

ImVec2 ImGui::GetFontTexUvWhitePixel() { return GImGui->FontTexUvWhitePixel; }

void ImGui::SetWindowFontScale(float scale)
{
    ImGuiContext& g         = *GImGui;
    ImGuiWindow*  window    = GetCurrentWindow();
    window->FontWindowScale = scale;
    g.FontSize              = window->CalcFontSize();
}

// User generally sees positions in window coordinates. Internally we store
// CursorPos in absolute screen coordinates because it is more convenient.
// Conversion happens as we pass the value to user, but it makes our naming
// convention confusing because GetCursorPos() == (DC.CursorPos - window.Pos).
// May want to rename 'DC.CursorPos'.
ImVec2 ImGui::GetCursorPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
=======
ImFont *ImGui::GetFont() { return GImGui->Font; }

float ImGui::GetFontSize() { return GImGui->FontSize; }

ImVec2 ImGui::GetFontTexUvWhitePixel() { return GImGui->FontTexUvWhitePixel; }

void ImGui::SetWindowFontScale(float scale) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();
  window->FontWindowScale = scale;
  g.FontSize = window->CalcFontSize();
}

// User generally sees positions in window coordinates. Internally we store
// CursorPos in absolute screen coordinates because it is more convenient.
// Conversion happens as we pass the value to user, but it makes our naming
// convention confusing because GetCursorPos() == (DC.CursorPos - window.Pos).
// May want to rename 'DC.CursorPos'.
ImVec2 ImGui::GetCursorPos() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.CursorPos - window->Pos + window->Scroll;
}

float ImGui::GetCursorPosX() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float ImGui::GetCursorPosY() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void ImGui::SetCursorPos(const ImVec2 &local_pos) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
  window->DC.CursorMaxPos =
      ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
>>>>>>> master
}

void ImGui::SetCursorPosX(float x) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
  window->DC.CursorMaxPos.x =
      ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
  window->DC.CursorMaxPos.y =
      ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
}

<<<<<<< HEAD
void ImGui::SetCursorPos(const ImVec2& local_pos)
{
    ImGuiWindow* window     = GetCurrentWindow();
    window->DC.CursorPos    = window->Pos - window->Scroll + local_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

void ImGui::SetCursorPosX(float x)
{
    ImGuiWindow* window       = GetCurrentWindow();
    window->DC.CursorPos.x    = window->Pos.x - window->Scroll.x + x;
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y)
{
    ImGuiWindow* window       = GetCurrentWindow();
    window->DC.CursorPos.y    = window->Pos.y - window->Scroll.y + y;
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
=======
ImVec2 ImGui::GetCursorStartPos() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.CursorStartPos - window->Pos;
}

ImVec2 ImGui::GetCursorScreenPos() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.CursorPos;
}

void ImGui::SetCursorScreenPos(const ImVec2 &screen_pos) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.CursorPos = screen_pos;
  window->DC.CursorMaxPos =
      ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
>>>>>>> master
}

float ImGui::GetScrollX() { return GImGui->CurrentWindow->Scroll.x; }

float ImGui::GetScrollY() { return GImGui->CurrentWindow->Scroll.y; }

float ImGui::GetScrollMaxX() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->SizeContents.x - window->SizeFull.x - window->ScrollbarSizes.x;
}

float ImGui::GetScrollMaxY() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->SizeContents.y - window->SizeFull.y - window->ScrollbarSizes.y;
}

<<<<<<< HEAD
void ImGui::SetCursorScreenPos(const ImVec2& screen_pos)
{
    ImGuiWindow* window     = GetCurrentWindow();
    window->DC.CursorPos    = screen_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

float ImGui::GetScrollX() { return GImGui->CurrentWindow->Scroll.x; }

float ImGui::GetScrollY() { return GImGui->CurrentWindow->Scroll.y; }
=======
void ImGui::SetScrollX(float scroll_x) {
  ImGuiWindow *window = GetCurrentWindow();
  window->ScrollTarget.x = scroll_x;
  window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y) {
  ImGuiWindow *window = GetCurrentWindow();
  window->ScrollTarget.y = scroll_y + window->TitleBarHeight() +
                           window->MenuBarHeight(); // title bar height canceled
                                                    // out when using
                                                    // ScrollTargetRelY
  window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio) {
  // We store a target position so centering can occur on the next frame when we
  // are guaranteed to have a known window size
  ImGuiWindow *window = GetCurrentWindow();
  IM_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
  window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
  if (center_y_ratio <= 0.0f &&
      window->ScrollTarget.y <=
          window->WindowPadding.y) // Minor hack to make "scroll to top" take
                                   // account of WindowPadding, else it would
                                   // scroll to (WindowPadding.y -
                                   // ItemSpacing.y)
    window->ScrollTarget.y = 0.0f;
  window->ScrollTargetCenterRatio.y = center_y_ratio;
}
>>>>>>> master

// center_y_ratio: 0.0f top of last item, 0.5f vertical center of last item,
// 1.0f bottom of last item.
void ImGui::SetScrollHere(float center_y_ratio) {
  ImGuiWindow *window = GetCurrentWindow();
  float target_y =
      window->DC.CursorPosPrevLine.y +
      (window->DC.PrevLineHeight * center_y_ratio) +
      (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) *
       2.0f); // Precisely aim above, in the middle or below the last line.
  SetScrollFromPosY(target_y - window->Pos.y, center_y_ratio);
}

void ImGui::SetKeyboardFocusHere(int offset) {
  ImGuiWindow *window = GetCurrentWindow();
  window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
  window->FocusIdxTabRequestNext = INT_MAX;
}

<<<<<<< HEAD
void ImGui::SetScrollX(float scroll_x)
{
    ImGuiWindow* window               = GetCurrentWindow();
    window->ScrollTarget.x            = scroll_x;
    window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->ScrollTarget.y =
        scroll_y + window->TitleBarHeight() + window->MenuBarHeight();  // title bar height canceled
                                                                        // out when using
                                                                        // ScrollTargetRelY
    window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
    // We store a target position so centering can occur on the next frame when we
    // are guaranteed to have a known window size
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
    window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
    if (center_y_ratio <= 0.0f &&
        window->ScrollTarget.y <= window->WindowPadding.y)  // Minor hack to make "scroll to top" take
                                                            // account of WindowPadding, else it would
                                                            // scroll to (WindowPadding.y -
                                                            // ItemSpacing.y)
        window->ScrollTarget.y        = 0.0f;
    window->ScrollTargetCenterRatio.y = center_y_ratio;
}

// center_y_ratio: 0.0f top of last item, 0.5f vertical center of last item,
// 1.0f bottom of last item.
void ImGui::SetScrollHere(float center_y_ratio)
{
    ImGuiWindow* window   = GetCurrentWindow();
    float        target_y = window->DC.CursorPosPrevLine.y + (window->DC.PrevLineHeight * center_y_ratio) +
                     (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) *
                         2.0f);  // Precisely aim above, in the middle or below the last line.
    SetScrollFromPosY(target_y - window->Pos.y, center_y_ratio);
}

void ImGui::SetKeyboardFocusHere(int offset)
{
    ImGuiWindow* window            = GetCurrentWindow();
    window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
    window->FocusIdxTabRequestNext = INT_MAX;
}

void ImGui::SetStateStorage(ImGuiStorage* tree)
{
    ImGuiWindow* window     = GetCurrentWindow();
    window->DC.StateStorage = tree ? tree : &window->StateStorage;
=======
void ImGui::SetStateStorage(ImGuiStorage *tree) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.StateStorage = tree ? tree : &window->StateStorage;
}

ImGuiStorage *ImGui::GetStateStorage() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.StateStorage;
}

void ImGui::TextV(const char *fmt, va_list args) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const char *text_end =
      g.TempBuffer +
      ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
  TextUnformatted(g.TempBuffer, text_end);
}

void ImGui::Text(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  TextV(fmt, args);
  va_end(args);
}

void ImGui::TextColoredV(const ImVec4 &col, const char *fmt, va_list args) {
  PushStyleColor(ImGuiCol_Text, col);
  TextV(fmt, args);
  PopStyleColor();
}

void ImGui::TextColored(const ImVec4 &col, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  TextColoredV(col, fmt, args);
  va_end(args);
>>>>>>> master
}

void ImGui::TextDisabledV(const char *fmt, va_list args) {
  PushStyleColor(ImGuiCol_Text, GImGui->Style.Colors[ImGuiCol_TextDisabled]);
  TextV(fmt, args);
  PopStyleColor();
}

<<<<<<< HEAD
void ImGui::TextV(const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g        = *GImGui;
    const char*   text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    TextUnformatted(g.TempBuffer, text_end);
=======
void ImGui::TextDisabled(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  TextDisabledV(fmt, args);
  va_end(args);
>>>>>>> master
}

void ImGui::TextWrappedV(const char *fmt, va_list args) {
  bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos <
                    0.0f); // Keep existing wrap position is one ia already set
  if (need_wrap)
    PushTextWrapPos(0.0f);
  TextV(fmt, args);
  if (need_wrap)
    PopTextWrapPos();
}

void ImGui::TextWrapped(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  TextWrappedV(fmt, args);
  va_end(args);
}

void ImGui::TextUnformatted(const char *text, const char *text_end) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  IM_ASSERT(text != NULL);
  const char *text_begin = text;
  if (text_end == NULL)
    text_end = text + strlen(text); // FIXME-OPT

  const float wrap_pos_x = window->DC.TextWrapPos;
  const bool wrap_enabled = wrap_pos_x >= 0.0f;
  if (text_end - text > 2000 && !wrap_enabled) {
    // Long text!
    // Perform manual coarse clipping to optimize for long multi-line text
    // From this point we will only compute the width of lines that are visible.
    // Optimization only available when word-wrapping is disabled.
    // We also don't vertically center the text within the line full height,
    // which is unlikely to matter because we are likely the biggest and only
    // item on the line.
    const char *line = text;
    const float line_height = GetTextLineHeight();
    const ImVec2 text_pos = window->DC.CursorPos +
                            ImVec2(0.0f, window->DC.CurrentLineTextBaseOffset);
    const ImRect clip_rect = window->ClipRect;
    ImVec2 text_size(0, 0);

<<<<<<< HEAD
void ImGui::TextWrappedV(const char* fmt, va_list args)
{
    bool need_wrap =
        (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);  // Keep existing wrap position is one ia already set
    if (need_wrap) PushTextWrapPos(0.0f);
    TextV(fmt, args);
    if (need_wrap) PopTextWrapPos();
}
=======
    if (text_pos.y <= clip_rect.Max.y) {
      ImVec2 pos = text_pos;
>>>>>>> master

      // Lines to skip (can't skip when logging text)
      if (!g.LogEnabled) {
        int lines_skippable =
            (int)((clip_rect.Min.y - text_pos.y) / line_height);
        if (lines_skippable > 0) {
          int lines_skipped = 0;
          while (line < text_end && lines_skipped < lines_skippable) {
            const char *line_end = strchr(line, '\n');
            if (!line_end)
              line_end = text_end;
            line = line_end + 1;
            lines_skipped++;
          }
          pos.y += lines_skipped * line_height;
        }
      }

<<<<<<< HEAD
void ImGui::TextUnformatted(const char* text, const char* text_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    IM_ASSERT(text != NULL);
    const char* text_begin         = text;
    if (text_end == NULL) text_end = text + strlen(text);  // FIXME-OPT

    const float wrap_pos_x   = window->DC.TextWrapPos;
    const bool  wrap_enabled = wrap_pos_x >= 0.0f;
    if (text_end - text > 2000 && !wrap_enabled) {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // From this point we will only compute the width of lines that are visible.
        // Optimization only available when word-wrapping is disabled.
        // We also don't vertically center the text within the line full height,
        // which is unlikely to matter because we are likely the biggest and only
        // item on the line.
        const char*  line        = text;
        const float  line_height = GetTextLineHeight();
        const ImVec2 text_pos    = window->DC.CursorPos + ImVec2(0.0f, window->DC.CurrentLineTextBaseOffset);
        const ImRect clip_rect   = window->ClipRect;
        ImVec2       text_size(0, 0);

        if (text_pos.y <= clip_rect.Max.y) {
            ImVec2 pos = text_pos;

            // Lines to skip (can't skip when logging text)
            if (!g.LogEnabled) {
                int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
                if (lines_skippable > 0) {
                    int lines_skipped = 0;
                    while (line < text_end && lines_skipped < lines_skippable) {
                        const char* line_end    = strchr(line, '\n');
                        if (!line_end) line_end = text_end;
                        line                    = line_end + 1;
                        lines_skipped++;
                    }
                    pos.y += lines_skipped * line_height;
                }
            }

            // Lines to render
            if (line < text_end) {
                ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
                while (line < text_end) {
                    const char* line_end = strchr(line, '\n');
                    if (IsClippedEx(line_rect, NULL, false)) break;

                    const ImVec2 line_size = CalcTextSize(line, line_end, false);
                    text_size.x            = ImMax(text_size.x, line_size.x);
                    RenderText(pos, line, line_end, false);
                    if (!line_end) line_end = text_end;
                    line                    = line_end + 1;
                    line_rect.Min.y += line_height;
                    line_rect.Max.y += line_height;
                    pos.y += line_height;
                }

                // Count remaining lines
                int lines_skipped = 0;
                while (line < text_end) {
                    const char* line_end    = strchr(line, '\n');
                    if (!line_end) line_end = text_end;
                    line                    = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }

            text_size.y += (pos - text_pos).y;
        }

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(bb);
        ItemAdd(bb, NULL);
    }
    else
    {
        const float  wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        const ImVec2 text_size  = CalcTextSize(text_begin, text_end, false, wrap_width);

        // Account of baseline offset
        ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);
        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size);
        if (!ItemAdd(bb, NULL)) return;

        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
=======
      // Lines to render
      if (line < text_end) {
        ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
        while (line < text_end) {
          const char *line_end = strchr(line, '\n');
          if (IsClippedEx(line_rect, NULL, false))
            break;

          const ImVec2 line_size = CalcTextSize(line, line_end, false);
          text_size.x = ImMax(text_size.x, line_size.x);
          RenderText(pos, line, line_end, false);
          if (!line_end)
            line_end = text_end;
          line = line_end + 1;
          line_rect.Min.y += line_height;
          line_rect.Max.y += line_height;
          pos.y += line_height;
        }

        // Count remaining lines
        int lines_skipped = 0;
        while (line < text_end) {
          const char *line_end = strchr(line, '\n');
          if (!line_end)
            line_end = text_end;
          line = line_end + 1;
          lines_skipped++;
        }
        pos.y += lines_skipped * line_height;
      }

      text_size.y += (pos - text_pos).y;
>>>>>>> master
    }

    ImRect bb(text_pos, text_pos + text_size);
    ItemSize(bb);
    ItemAdd(bb, NULL);
  } else {
    const float wrap_width =
        wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x)
                     : 0.0f;
    const ImVec2 text_size =
        CalcTextSize(text_begin, text_end, false, wrap_width);

    // Account of baseline offset
    ImVec2 text_pos(window->DC.CursorPos.x,
                    window->DC.CursorPos.y +
                        window->DC.CurrentLineTextBaseOffset);
    ImRect bb(text_pos, text_pos + text_size);
    ItemSize(text_size);
    if (!ItemAdd(bb, NULL))
      return;

    // Render (we don't hide text after ## in this end-user function)
    RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
  }
}

<<<<<<< HEAD
void ImGui::AlignFirstTextHeightToWidgets()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    // Declare a dummy item size to that upcoming items that are smaller will
    // center-align on the newly expanded line height.
    ImGuiContext& g = *GImGui;
    ItemSize(ImVec2(0, g.FontSize + g.Style.FramePadding.y * 2), g.Style.FramePadding.y);
    SameLine(0, 0);
}

// Add a label+text combo aligned to other label+value widgets
void ImGui::LabelTextV(const char* label, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float       w     = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect value_bb(
        window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2));
    const ImRect total_bb(window->DC.CursorPos,
        window->DC.CursorPos +
            ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y * 2) + label_size);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, NULL)) return;

    // Render
    const char* value_text_begin = &g.TempBuffer[0];
    const char* value_text_end =
        value_text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin, value_text_end, NULL, ImVec2(0.0f, 0.5f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);
=======
void ImGui::AlignFirstTextHeightToWidgets() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  // Declare a dummy item size to that upcoming items that are smaller will
  // center-align on the newly expanded line height.
  ImGuiContext &g = *GImGui;
  ItemSize(ImVec2(0, g.FontSize + g.Style.FramePadding.y * 2),
           g.Style.FramePadding.y);
  SameLine(0, 0);
}

// Add a label+text combo aligned to other label+value widgets
void ImGui::LabelTextV(const char *label, const char *fmt, va_list args) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const float w = CalcItemWidth();

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect value_bb(window->DC.CursorPos,
                        window->DC.CursorPos +
                            ImVec2(w, label_size.y + style.FramePadding.y * 2));
  const ImRect total_bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f),
                 style.FramePadding.y * 2) +
          label_size);
  ItemSize(total_bb, style.FramePadding.y);
  if (!ItemAdd(total_bb, NULL))
    return;

  // Render
  const char *value_text_begin = &g.TempBuffer[0];
  const char *value_text_end =
      value_text_begin +
      ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
  RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin,
                    value_text_end, NULL, ImVec2(0.0f, 0.5f));
  if (label_size.x > 0.0f)
    RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x,
                      value_bb.Min.y + style.FramePadding.y),
               label);
>>>>>>> master
}

void ImGui::LabelText(const char *label, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  LabelTextV(label, fmt, args);
  va_end(args);
}

<<<<<<< HEAD
static inline bool IsWindowContentHoverable(ImGuiWindow* window)
{
    // An active popup disable hovering on other windows (apart from its own
    // children)
    ImGuiContext& g = *GImGui;
    if (ImGuiWindow* focused_window = g.FocusedWindow)
        if (ImGuiWindow* focused_root_window = focused_window->RootWindow)
            if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) != 0 && focused_root_window->WasActive &&
                focused_root_window != window->RootWindow)
                return false;

    return true;
}

bool ImGui::ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    if (flags & ImGuiButtonFlags_Disabled) {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held       = false;
        if (g.ActiveId == id) ClearActiveID();
=======
static inline bool IsWindowContentHoverable(ImGuiWindow *window) {
  // An active popup disable hovering on other windows (apart from its own
  // children)
  ImGuiContext &g = *GImGui;
  if (ImGuiWindow *focused_window = g.FocusedWindow)
    if (ImGuiWindow *focused_root_window = focused_window->RootWindow)
      if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) != 0 &&
          focused_root_window->WasActive &&
          focused_root_window != window->RootWindow)
>>>>>>> master
        return false;

<<<<<<< HEAD
    if ((flags & (ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClick |
                     ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick)) == 0)
        flags |= ImGuiButtonFlags_PressedOnClickRelease;

    bool pressed = false;
    bool hovered = IsHovered(bb, id, (flags & ImGuiButtonFlags_FlattenChilds) != 0);
    if (hovered) {
        SetHoveredID(id);
        if (!(flags & ImGuiButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt)) {
            //                        | CLICKING        | HOLDING with
            //                        ImGuiButtonFlags_Repeat
            // PressedOnClickRelease  |  <on release>*  |  <on repeat> <on repeat> ..
            // (NOT on release)  <-- MOST COMMON! (*) only if both click/release were
            // over bounds
            // PressedOnClick         |  <on click>     |  <on click> <on repeat> <on
            // repeat> ..
            // PressedOnRelease       |  <on release>   |  <on repeat> <on repeat> ..
            // (NOT on release)
            // PressedOnDoubleClick   |  <on dclick>    |  <on dclick> <on repeat> <on
            // repeat> ..
            if ((flags & ImGuiButtonFlags_PressedOnClickRelease) && g.IO.MouseClicked[0]) {
                SetActiveID(id, window);  // Hold on ID
                FocusWindow(window);
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
            }
            if (((flags & ImGuiButtonFlags_PressedOnClick) && g.IO.MouseClicked[0]) ||
                ((flags & ImGuiButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[0]))
            {
                pressed = true;
                ClearActiveID();
                FocusWindow(window);
            }
            if ((flags & ImGuiButtonFlags_PressedOnRelease) && g.IO.MouseReleased[0]) {
                if (!((flags & ImGuiButtonFlags_Repeat) &&
                        g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))  // Repeat mode trumps <on release>
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table
            // above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it
            // ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id && g.IO.MouseDownDuration[0] > 0.0f &&
                IsMouseClicked(0, true))
                pressed = true;
        }
    }

    bool held = false;
    if (g.ActiveId == id) {
        if (g.IO.MouseDown[0]) {
            held = true;
        }
        else
        {
            if (hovered && (flags & ImGuiButtonFlags_PressedOnClickRelease))
                if (!((flags & ImGuiButtonFlags_Repeat) &&
                        g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))  // Repeat mode trumps <on release>
                    pressed = true;
            ClearActiveID();
        }
=======
  return true;
}

bool ImGui::ButtonBehavior(const ImRect &bb, ImGuiID id, bool *out_hovered,
                           bool *out_held, ImGuiButtonFlags flags) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  if (flags & ImGuiButtonFlags_Disabled) {
    if (out_hovered)
      *out_hovered = false;
    if (out_held)
      *out_held = false;
    if (g.ActiveId == id)
      ClearActiveID();
    return false;
  }

  if ((flags &
       (ImGuiButtonFlags_PressedOnClickRelease |
        ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease |
        ImGuiButtonFlags_PressedOnDoubleClick)) == 0)
    flags |= ImGuiButtonFlags_PressedOnClickRelease;

  bool pressed = false;
  bool hovered =
      IsHovered(bb, id, (flags & ImGuiButtonFlags_FlattenChilds) != 0);
  if (hovered) {
    SetHoveredID(id);
    if (!(flags & ImGuiButtonFlags_NoKeyModifiers) ||
        (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt)) {
      //                        | CLICKING        | HOLDING with
      //                        ImGuiButtonFlags_Repeat
      // PressedOnClickRelease  |  <on release>*  |  <on repeat> <on repeat> ..
      // (NOT on release)  <-- MOST COMMON! (*) only if both click/release were
      // over bounds
      // PressedOnClick         |  <on click>     |  <on click> <on repeat> <on
      // repeat> ..
      // PressedOnRelease       |  <on release>   |  <on repeat> <on repeat> ..
      // (NOT on release)
      // PressedOnDoubleClick   |  <on dclick>    |  <on dclick> <on repeat> <on
      // repeat> ..
      if ((flags & ImGuiButtonFlags_PressedOnClickRelease) &&
          g.IO.MouseClicked[0]) {
        SetActiveID(id, window); // Hold on ID
        FocusWindow(window);
        g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
      }
      if (((flags & ImGuiButtonFlags_PressedOnClick) && g.IO.MouseClicked[0]) ||
          ((flags & ImGuiButtonFlags_PressedOnDoubleClick) &&
           g.IO.MouseDoubleClicked[0])) {
        pressed = true;
        ClearActiveID();
        FocusWindow(window);
      }
      if ((flags & ImGuiButtonFlags_PressedOnRelease) &&
          g.IO.MouseReleased[0]) {
        if (!((flags & ImGuiButtonFlags_Repeat) &&
              g.IO.MouseDownDurationPrev[0] >=
                  g.IO.KeyRepeatDelay)) // Repeat mode trumps <on release>
          pressed = true;
        ClearActiveID();
      }

      // 'Repeat' mode acts when held regardless of _PressedOn flags (see table
      // above).
      // Relies on repeat logic of IsMouseClicked() but we may as well do it
      // ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
      if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id &&
          g.IO.MouseDownDuration[0] > 0.0f && IsMouseClicked(0, true))
        pressed = true;
>>>>>>> master
    }
  }

<<<<<<< HEAD
    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be
    // null or to match. This allows using patterns where a later submitted widget
    // overlaps a previous one.
    if (hovered && (flags & ImGuiButtonFlags_AllowOverlapMode) &&
        (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = pressed = held = false;

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held       = held;
=======
  bool held = false;
  if (g.ActiveId == id) {
    if (g.IO.MouseDown[0]) {
      held = true;
    } else {
      if (hovered && (flags & ImGuiButtonFlags_PressedOnClickRelease))
        if (!((flags & ImGuiButtonFlags_Repeat) &&
              g.IO.MouseDownDurationPrev[0] >=
                  g.IO.KeyRepeatDelay)) // Repeat mode trumps <on release>
          pressed = true;
      ClearActiveID();
    }
  }

  // AllowOverlap mode (rarely used) requires previous frame HoveredId to be
  // null or to match. This allows using patterns where a later submitted widget
  // overlaps a previous one.
  if (hovered && (flags & ImGuiButtonFlags_AllowOverlapMode) &&
      (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
    hovered = pressed = held = false;
>>>>>>> master

  if (out_hovered)
    *out_hovered = hovered;
  if (out_held)
    *out_held = held;

  return pressed;
}

<<<<<<< HEAD
bool ImGui::ButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g          = *GImGui;
    const ImGuiStyle& style      = g.Style;
    const ImGuiID     id         = window->GetID(label);
    const ImVec2      label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
        style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)  // Try to vertically align
                                                                      // buttons that are smaller/have
                                                                      // no padding so that text
                                                                      // baseline matches (bit hacky,
                                                                      // since it shouldn't be a flag)
        pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size =
        CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, &id)) return false;
=======
bool ImGui::ButtonEx(const char *label, const ImVec2 &size_arg,
                     ImGuiButtonFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  ImVec2 pos = window->DC.CursorPos;
  if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
      style.FramePadding.y <
          window->DC.CurrentLineTextBaseOffset) // Try to vertically align
                                                // buttons that are smaller/have
                                                // no padding so that text
                                                // baseline matches (bit hacky,
                                                // since it shouldn't be a flag)
    pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
  ImVec2 size =
      CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f,
                   label_size.y + style.FramePadding.y * 2.0f);

  const ImRect bb(pos, pos + size);
  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(bb, &id))
    return false;
>>>>>>> master

  if (window->DC.ButtonRepeat)
    flags |= ImGuiButtonFlags_Repeat;
  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

<<<<<<< HEAD
    // Render
    const ImU32 col =
        GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    RenderTextClipped(
        bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    // if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) &&
    // (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();
=======
  // Render
  const ImU32 col = GetColorU32(
      (hovered && held) ? ImGuiCol_ButtonActive
                        : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
  RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
  RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding,
                    label, NULL, &label_size, style.ButtonTextAlign, &bb);

  // Automatically close popups
  // if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) &&
  // (window->Flags & ImGuiWindowFlags_Popup))
  //    CloseCurrentPopup();
>>>>>>> master

  return pressed;
}

<<<<<<< HEAD
bool ImGui::Button(const char* label, const ImVec2& size_arg) { return ButtonEx(label, size_arg, 0); }

// Small buttons fits within text without additional vertical spacing.
bool ImGui::SmallButton(const char* label)
{
    ImGuiContext& g                = *GImGui;
    float         backup_padding_y = g.Style.FramePadding.y;
    g.Style.FramePadding.y         = 0.0f;
    bool pressed                   = ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine);
    g.Style.FramePadding.y         = backup_padding_y;
    return pressed;
=======
bool ImGui::Button(const char *label, const ImVec2 &size_arg) {
  return ButtonEx(label, size_arg, 0);
}

// Small buttons fits within text without additional vertical spacing.
bool ImGui::SmallButton(const char *label) {
  ImGuiContext &g = *GImGui;
  float backup_padding_y = g.Style.FramePadding.y;
  g.Style.FramePadding.y = 0.0f;
  bool pressed =
      ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine);
  g.Style.FramePadding.y = backup_padding_y;
  return pressed;
>>>>>>> master
}

// Tip: use ImGui::PushID()/PopID() to push indices or pointers in the ID stack.
// Then you can keep 'str_id' empty or the same for all your buttons (instead of
// creating a string based on a non-string id)
<<<<<<< HEAD
bool ImGui::InvisibleButton(const char* str_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    const ImGuiID id   = window->GetID(str_id);
    ImVec2        size = CalcItemSize(size_arg, 0.0f, 0.0f);
    const ImRect  bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, &id)) return false;
=======
bool ImGui::InvisibleButton(const char *str_id, const ImVec2 &size_arg) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  const ImGuiID id = window->GetID(str_id);
  ImVec2 size = CalcItemSize(size_arg, 0.0f, 0.0f);
  const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
  ItemSize(bb);
  if (!ItemAdd(bb, &id))
    return false;
>>>>>>> master

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held);

  return pressed;
}

// Upper-right button to close a window.
bool ImGui::CloseButton(ImGuiID id, const ImVec2 &pos, float radius) {
  ImGuiWindow *window = GetCurrentWindow();

<<<<<<< HEAD
    const ImRect bb(pos - ImVec2(radius, radius), pos + ImVec2(radius, radius));
=======
  const ImRect bb(pos - ImVec2(radius, radius), pos + ImVec2(radius, radius));
>>>>>>> master

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held);

<<<<<<< HEAD
    // Render
    const ImU32 col = GetColorU32(
        (held && hovered) ? ImGuiCol_CloseButtonActive : hovered ? ImGuiCol_CloseButtonHovered : ImGuiCol_CloseButton);
    const ImVec2 center = bb.GetCenter();
    window->DrawList->AddCircleFilled(center, ImMax(2.0f, radius), col, 12);

    const float cross_extent = (radius * 0.7071f) - 1.0f;
    if (hovered) {
        window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent),
            center + ImVec2(-cross_extent, -cross_extent),
            GetColorU32(ImGuiCol_Text));
        window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent),
            center + ImVec2(-cross_extent, +cross_extent),
            GetColorU32(ImGuiCol_Text));
    }
=======
  // Render
  const ImU32 col =
      GetColorU32((held && hovered) ? ImGuiCol_CloseButtonActive
                                    : hovered ? ImGuiCol_CloseButtonHovered
                                              : ImGuiCol_CloseButton);
  const ImVec2 center = bb.GetCenter();
  window->DrawList->AddCircleFilled(center, ImMax(2.0f, radius), col, 12);

  const float cross_extent = (radius * 0.7071f) - 1.0f;
  if (hovered) {
    window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent),
                              center + ImVec2(-cross_extent, -cross_extent),
                              GetColorU32(ImGuiCol_Text));
    window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent),
                              center + ImVec2(-cross_extent, +cross_extent),
                              GetColorU32(ImGuiCol_Text));
  }
>>>>>>> master

  return pressed;
}

<<<<<<< HEAD
void ImGui::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
    const ImVec4& tint_col, const ImVec4& border_col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (border_col.w > 0.0f) bb.Max += ImVec2(2, 2);
    ItemSize(bb);
    if (!ItemAdd(bb, NULL)) return;

    if (border_col.w > 0.0f) {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
        window->DrawList->AddImage(
            user_texture_id, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col));
    }
    else
    {
        window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
    }
=======
void ImGui::Image(ImTextureID user_texture_id, const ImVec2 &size,
                  const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tint_col,
                  const ImVec4 &border_col) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
  if (border_col.w > 0.0f)
    bb.Max += ImVec2(2, 2);
  ItemSize(bb);
  if (!ItemAdd(bb, NULL))
    return;

  if (border_col.w > 0.0f) {
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
    window->DrawList->AddImage(user_texture_id, bb.Min + ImVec2(1, 1),
                               bb.Max - ImVec2(1, 1), uv0, uv1,
                               GetColorU32(tint_col));
  } else {
    window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1,
                               GetColorU32(tint_col));
  }
>>>>>>> master
}

// frame_padding < 0: uses FramePadding from style (default)
// frame_padding = 0: no framing
// frame_padding > 0: set framing size
// The color used are the button colors.
<<<<<<< HEAD
bool ImGui::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
    int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Default to using texture ID as ID. User can still push string/integer
    // prefixes.
    // We could hash the size/uv to create a unique ID but that would prevent the
    // user from animating UV.
    PushID((void*)user_texture_id);
    const ImGuiID id = window->GetID("#image");
    PopID();

    const ImVec2 padding =
        (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, &id)) return false;
=======
bool ImGui::ImageButton(ImTextureID user_texture_id, const ImVec2 &size,
                        const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding,
                        const ImVec4 &bg_col, const ImVec4 &tint_col) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  // Default to using texture ID as ID. User can still push string/integer
  // prefixes.
  // We could hash the size/uv to create a unique ID but that would prevent the
  // user from animating UV.
  PushID((void *)user_texture_id);
  const ImGuiID id = window->GetID("#image");
  PopID();

  const ImVec2 padding =
      (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding)
                           : style.FramePadding;
  const ImRect bb(window->DC.CursorPos,
                  window->DC.CursorPos + size + padding * 2);
  const ImRect image_bb(window->DC.CursorPos + padding,
                        window->DC.CursorPos + padding + size);
  ItemSize(bb);
  if (!ItemAdd(bb, &id))
    return false;
>>>>>>> master

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held);

<<<<<<< HEAD
    // Render
    const ImU32 col =
        GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f) window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
    window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
=======
  // Render
  const ImU32 col = GetColorU32(
      (hovered && held) ? ImGuiCol_ButtonActive
                        : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
  RenderFrame(
      bb.Min, bb.Max, col, true,
      ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
  if (bg_col.w > 0.0f)
    window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max,
                                    GetColorU32(bg_col));
  window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0,
                             uv1, GetColorU32(tint_col));
>>>>>>> master

  return pressed;
}

// Start logging ImGui output to TTY
<<<<<<< HEAD
void ImGui::LogToTTY(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled) return;
    ImGuiWindow* window = GetCurrentWindowRead();

    g.LogEnabled                                = true;
    g.LogFile                                   = NULL;
    g.LogStartDepth                             = window->DC.TreeDepth;
    if (max_depth >= 0) g.LogAutoExpandMaxDepth = max_depth;
}

// Start logging ImGui output to given file
void ImGui::LogToFile(int max_depth, const char* filename) {}

// Start logging ImGui output to clipboard
void ImGui::LogToClipboard(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled) return;
    ImGuiWindow* window = GetCurrentWindowRead();

    g.LogEnabled                                = true;
    g.LogFile                                   = NULL;
    g.LogStartDepth                             = window->DC.TreeDepth;
    if (max_depth >= 0) g.LogAutoExpandMaxDepth = max_depth;
=======
void ImGui::LogToTTY(int max_depth) {
  ImGuiContext &g = *GImGui;
  if (g.LogEnabled)
    return;
  ImGuiWindow *window = GetCurrentWindowRead();

  g.LogEnabled = true;
  g.LogFile = NULL;
  g.LogStartDepth = window->DC.TreeDepth;
  if (max_depth >= 0)
    g.LogAutoExpandMaxDepth = max_depth;
}

// Start logging ImGui output to given file
void ImGui::LogToFile(int max_depth, const char *filename) {}

// Start logging ImGui output to clipboard
void ImGui::LogToClipboard(int max_depth) {
  ImGuiContext &g = *GImGui;
  if (g.LogEnabled)
    return;
  ImGuiWindow *window = GetCurrentWindowRead();

  g.LogEnabled = true;
  g.LogFile = NULL;
  g.LogStartDepth = window->DC.TreeDepth;
  if (max_depth >= 0)
    g.LogAutoExpandMaxDepth = max_depth;
>>>>>>> master
}

void ImGui::LogFinish() {}

// Helper to display logging buttons
void ImGui::LogButtons() {
  ImGuiContext &g = *GImGui;

<<<<<<< HEAD
    PushID("LogButtons");
    const bool log_to_tty = Button("Log To TTY");
    SameLine();
    const bool log_to_file = Button("Log To File");
    SameLine();
    const bool log_to_clipboard = Button("Log To Clipboard");
    SameLine();
    PushItemWidth(80.0f);
    PushAllowKeyboardFocus(false);
    SliderInt("Depth", &g.LogAutoExpandMaxDepth, 0, 9, NULL);
    PopAllowKeyboardFocus();
    PopItemWidth();
    PopID();

    // Start logging at the end of the function so that the buttons don't appear
    // in the log
    if (log_to_tty) LogToTTY(g.LogAutoExpandMaxDepth);
    if (log_to_file) LogToFile(g.LogAutoExpandMaxDepth, g.IO.LogFilename);
    if (log_to_clipboard) LogToClipboard(g.LogAutoExpandMaxDepth);
}

bool ImGui::TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags)
{
    if (flags & ImGuiTreeNodeFlags_Leaf) return true;

    // We only write to the tree storage if the user clicks (or explicitely use
    // SetNextTreeNode*** functions)
    ImGuiContext& g       = *GImGui;
    ImGuiWindow*  window  = g.CurrentWindow;
    ImGuiStorage* storage = window->DC.StateStorage;

    bool is_open;
    if (g.SetNextTreeNodeOpenCond != 0) {
        if (g.SetNextTreeNodeOpenCond & ImGuiSetCond_Always) {
            is_open = g.SetNextTreeNodeOpenVal;
            storage->SetInt(id, is_open);
        }
        else
        {
            // We treat ImGuiSetCondition_Once and ImGuiSetCondition_FirstUseEver the
            // same because tree node state are not saved persistently.
            const int stored_value = storage->GetInt(id, -1);
            if (stored_value == -1) {
                is_open = g.SetNextTreeNodeOpenVal;
                storage->SetInt(id, is_open);
            }
            else
            {
                is_open = stored_value != 0;
            }
        }
        g.SetNextTreeNodeOpenCond = 0;
    }
    else
    {
        is_open = storage->GetInt(id, (flags & ImGuiTreeNodeFlags_DefaultOpen) ? 1 : 0) != 0;
=======
  PushID("LogButtons");
  const bool log_to_tty = Button("Log To TTY");
  SameLine();
  const bool log_to_file = Button("Log To File");
  SameLine();
  const bool log_to_clipboard = Button("Log To Clipboard");
  SameLine();
  PushItemWidth(80.0f);
  PushAllowKeyboardFocus(false);
  SliderInt("Depth", &g.LogAutoExpandMaxDepth, 0, 9, NULL);
  PopAllowKeyboardFocus();
  PopItemWidth();
  PopID();

  // Start logging at the end of the function so that the buttons don't appear
  // in the log
  if (log_to_tty)
    LogToTTY(g.LogAutoExpandMaxDepth);
  if (log_to_file)
    LogToFile(g.LogAutoExpandMaxDepth, g.IO.LogFilename);
  if (log_to_clipboard)
    LogToClipboard(g.LogAutoExpandMaxDepth);
}

bool ImGui::TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags) {
  if (flags & ImGuiTreeNodeFlags_Leaf)
    return true;

  // We only write to the tree storage if the user clicks (or explicitely use
  // SetNextTreeNode*** functions)
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = g.CurrentWindow;
  ImGuiStorage *storage = window->DC.StateStorage;

  bool is_open;
  if (g.SetNextTreeNodeOpenCond != 0) {
    if (g.SetNextTreeNodeOpenCond & ImGuiSetCond_Always) {
      is_open = g.SetNextTreeNodeOpenVal;
      storage->SetInt(id, is_open);
    } else {
      // We treat ImGuiSetCondition_Once and ImGuiSetCondition_FirstUseEver the
      // same because tree node state are not saved persistently.
      const int stored_value = storage->GetInt(id, -1);
      if (stored_value == -1) {
        is_open = g.SetNextTreeNodeOpenVal;
        storage->SetInt(id, is_open);
      } else {
        is_open = stored_value != 0;
      }
>>>>>>> master
    }
    g.SetNextTreeNodeOpenCond = 0;
  } else {
    is_open = storage->GetInt(
                  id, (flags & ImGuiTreeNodeFlags_DefaultOpen) ? 1 : 0) != 0;
  }

<<<<<<< HEAD
    // When logging is enabled, we automatically expand tree nodes (but *NOT*
    // collapsing headers.. seems like sensible behavior).
    // NB- If we are above max depth we still allow manually opened nodes to be
    // logged.
    if (g.LogEnabled && !(flags & ImGuiTreeNodeFlags_NoAutoOpenOnLog) && window->DC.TreeDepth < g.LogAutoExpandMaxDepth)
        is_open = true;
=======
  // When logging is enabled, we automatically expand tree nodes (but *NOT*
  // collapsing headers.. seems like sensible behavior).
  // NB- If we are above max depth we still allow manually opened nodes to be
  // logged.
  if (g.LogEnabled && !(flags & ImGuiTreeNodeFlags_NoAutoOpenOnLog) &&
      window->DC.TreeDepth < g.LogAutoExpandMaxDepth)
    is_open = true;
>>>>>>> master

  return is_open;
}

<<<<<<< HEAD
bool ImGui::TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g             = *GImGui;
    const ImGuiStyle& style         = g.Style;
    const bool        display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2      padding       = display_frame ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

    if (!label_end) label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size   = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float text_base_offset_y =
        ImMax(0.0f, window->DC.CurrentLineTextBaseOffset - padding.y);  // Latch before ItemSize changes it
    const float frame_height =
        ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
    ImRect bb = ImRect(
        window->DC.CursorPos, ImVec2(window->Pos.x + GetContentRegionMax().x, window->DC.CursorPos.y + frame_height));
    if (display_frame) {
        // Framed header expand a little outside the default padding
        bb.Min.x -= (float)(int)(window->WindowPadding.x * 0.5f) - 1;
        bb.Max.x += (float)(int)(window->WindowPadding.x * 0.5f) - 1;
    }

    const float text_offset_x =
        (g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2));  // Collapser arrow width + Spacing
    const float text_width =
        g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
    ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of
    // ItemSpacing
    // (Ideally we'd want to add a flag for the user to specify we want want the
    // hit test to be done up to the right side of the content or not)
    const ImRect interact_bb =
        display_frame ? bb : ImRect(bb.Min.x, bb.Min.y, bb.Min.x + text_width + style.ItemSpacing.x * 2, bb.Max.y);
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);
    if (!ItemAdd(interact_bb, &id)) {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) TreePushRawID(id);
        return is_open;
    }

    // Flags that affects opening behavior:
    // - 0(default) ..................... single-click anywhere to open
    // - OpenOnDoubleClick .............. double-click anywhere to open
    // - OpenOnArrow .................... single-click on arrow to open
    // - OpenOnDoubleClick|OpenOnArrow .. single-click on arrow or double-click
    // anywhere to open
    ImGuiButtonFlags button_flags =
        ImGuiButtonFlags_NoKeyModifiers |
        ((flags & ImGuiTreeNodeFlags_AllowOverlapMode) ? ImGuiButtonFlags_AllowOverlapMode : 0);
    if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnDoubleClick |
                        ((flags & ImGuiTreeNodeFlags_OpenOnArrow) ? ImGuiButtonFlags_PressedOnClickRelease : 0);
    bool hovered, held, pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    if (pressed && !(flags & ImGuiTreeNodeFlags_Leaf)) {
        bool toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick));
        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
            toggled |=
                IsMouseHoveringRect(interact_bb.Min, ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y));
        if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) toggled |= g.IO.MouseDoubleClicked[0];
        if (toggled) {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowOverlapMode) SetItemAllowOverlap();

    // Render
    const ImU32 col =
        GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
    const ImVec2 text_pos = bb.Min + ImVec2(text_offset_x, padding.y + text_base_offset_y);
    if (display_frame) {
        // Framed type
        RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
        RenderCollapseTriangle(bb.Min + padding + ImVec2(0.0f, text_base_offset_y), is_open, 1.0f);
        if (g.LogEnabled) {
            // NB: '##' is normally used to hide text (as a library-wide feature), so
            // we need to specify the text range to make sure the ## aren't stripped
            // out here.
            const char log_prefix[] = "\n##";
            const char log_suffix[] = "##";
            LogRenderedText(text_pos, log_prefix, log_prefix + 3);
            RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
            LogRenderedText(text_pos, log_suffix + 1, log_suffix + 3);
        }
        else
        {
            RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
        }
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || (flags & ImGuiTreeNodeFlags_Selected)) RenderFrame(bb.Min, bb.Max, col, false);

        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(bb.Min + ImVec2(text_offset_x * 0.5f, g.FontSize * 0.50f + text_base_offset_y));
        else if (!(flags & ImGuiTreeNodeFlags_Leaf))
            RenderCollapseTriangle(bb.Min + ImVec2(padding.x, g.FontSize * 0.15f + text_base_offset_y), is_open, 0.70f);
        if (g.LogEnabled) LogRenderedText(text_pos, ">");
        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) TreePushRawID(id);
=======
bool ImGui::TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags,
                             const char *label, const char *label_end) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
  const ImVec2 padding =
      display_frame ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

  if (!label_end)
    label_end = FindRenderedTextEnd(label);
  const ImVec2 label_size = CalcTextSize(label, label_end, false);

  // We vertically grow up to current line height up the typical widget height.
  const float text_base_offset_y =
      ImMax(0.0f, window->DC.CurrentLineTextBaseOffset -
                      padding.y); // Latch before ItemSize changes it
  const float frame_height = ImMax(ImMin(window->DC.CurrentLineHeight,
                                         g.FontSize + style.FramePadding.y * 2),
                                   label_size.y + padding.y * 2);
  ImRect bb = ImRect(window->DC.CursorPos,
                     ImVec2(window->Pos.x + GetContentRegionMax().x,
                            window->DC.CursorPos.y + frame_height));
  if (display_frame) {
    // Framed header expand a little outside the default padding
    bb.Min.x -= (float)(int)(window->WindowPadding.x * 0.5f) - 1;
    bb.Max.x += (float)(int)(window->WindowPadding.x * 0.5f) - 1;
  }

  const float text_offset_x =
      (g.FontSize + (display_frame
                         ? padding.x * 3
                         : padding.x * 2)); // Collapser arrow width + Spacing
  const float text_width =
      g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2
                                        : 0.0f); // Include collapser
  ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

  // For regular tree nodes, we arbitrary allow to click past 2 worth of
  // ItemSpacing
  // (Ideally we'd want to add a flag for the user to specify we want want the
  // hit test to be done up to the right side of the content or not)
  const ImRect interact_bb =
      display_frame
          ? bb
          : ImRect(bb.Min.x, bb.Min.y,
                   bb.Min.x + text_width + style.ItemSpacing.x * 2, bb.Max.y);
  bool is_open = TreeNodeBehaviorIsOpen(id, flags);
  if (!ItemAdd(interact_bb, &id)) {
    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
      TreePushRawID(id);
>>>>>>> master
    return is_open;
  }

<<<<<<< HEAD
// CollapsingHeader returns true when opened but do not indent nor push into the
// ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label,
// ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen).
// You can remove the _NoTreePushOnOpen flag if you want behavior closer to
// normal TreeNode().
bool ImGui::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    return TreeNodeBehavior(
        window->GetID(label), flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen, label);
}

bool ImGui::CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    if (p_open && !*p_open) return false;

    ImGuiID id      = window->GetID(label);
    bool    is_open = TreeNodeBehavior(id,
        flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen |
            (p_open ? ImGuiTreeNodeFlags_AllowOverlapMode : 0),
        label);
    if (p_open) {
        // Create a small overlapping close button // FIXME: We can evolve this into
        // user accessible helpers to add extra buttons on title bars, headers, etc.
        ImGuiContext& g         = *GImGui;
        float         button_sz = g.FontSize * 0.5f;
        if (CloseButton(window->GetID((void*)(intptr_t)(id + 1)),
                ImVec2(ImMin(window->DC.LastItemRect.Max.x, window->ClipRect.Max.x) - g.Style.FramePadding.x -
                                button_sz,
                            window->DC.LastItemRect.Min.y + g.Style.FramePadding.y + button_sz),
                button_sz))
            *p_open = false;
=======
  // Flags that affects opening behavior:
  // - 0(default) ..................... single-click anywhere to open
  // - OpenOnDoubleClick .............. double-click anywhere to open
  // - OpenOnArrow .................... single-click on arrow to open
  // - OpenOnDoubleClick|OpenOnArrow .. single-click on arrow or double-click
  // anywhere to open
  ImGuiButtonFlags button_flags = ImGuiButtonFlags_NoKeyModifiers |
                                  ((flags & ImGuiTreeNodeFlags_AllowOverlapMode)
                                       ? ImGuiButtonFlags_AllowOverlapMode
                                       : 0);
  if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
    button_flags |= ImGuiButtonFlags_PressedOnDoubleClick |
                    ((flags & ImGuiTreeNodeFlags_OpenOnArrow)
                         ? ImGuiButtonFlags_PressedOnClickRelease
                         : 0);
  bool hovered, held,
      pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
  if (pressed && !(flags & ImGuiTreeNodeFlags_Leaf)) {
    bool toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow |
                              ImGuiTreeNodeFlags_OpenOnDoubleClick));
    if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
      toggled |= IsMouseHoveringRect(
          interact_bb.Min,
          ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y));
    if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
      toggled |= g.IO.MouseDoubleClicked[0];
    if (toggled) {
      is_open = !is_open;
      window->DC.StateStorage->SetInt(id, is_open);
>>>>>>> master
    }
  }
  if (flags & ImGuiTreeNodeFlags_AllowOverlapMode)
    SetItemAllowOverlap();

  // Render
  const ImU32 col = GetColorU32(
      (held && hovered) ? ImGuiCol_HeaderActive
                        : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
  const ImVec2 text_pos =
      bb.Min + ImVec2(text_offset_x, padding.y + text_base_offset_y);
  if (display_frame) {
    // Framed type
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    RenderCollapseTriangle(bb.Min + padding + ImVec2(0.0f, text_base_offset_y),
                           is_open, 1.0f);
    if (g.LogEnabled) {
      // NB: '##' is normally used to hide text (as a library-wide feature), so
      // we need to specify the text range to make sure the ## aren't stripped
      // out here.
      const char log_prefix[] = "\n##";
      const char log_suffix[] = "##";
      LogRenderedText(text_pos, log_prefix, log_prefix + 3);
      RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
      LogRenderedText(text_pos, log_suffix + 1, log_suffix + 3);
    } else {
      RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
    }
  } else {
    // Unframed typed for tree nodes
    if (hovered || (flags & ImGuiTreeNodeFlags_Selected))
      RenderFrame(bb.Min, bb.Max, col, false);

    if (flags & ImGuiTreeNodeFlags_Bullet)
      RenderBullet(bb.Min + ImVec2(text_offset_x * 0.5f,
                                   g.FontSize * 0.50f + text_base_offset_y));
    else if (!(flags & ImGuiTreeNodeFlags_Leaf))
      RenderCollapseTriangle(
          bb.Min + ImVec2(padding.x, g.FontSize * 0.15f + text_base_offset_y),
          is_open, 0.70f);
    if (g.LogEnabled)
      LogRenderedText(text_pos, ">");
    RenderText(text_pos, label, label_end, false);
  }

  if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
    TreePushRawID(id);
  return is_open;
}

<<<<<<< HEAD
bool ImGui::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;
=======
// CollapsingHeader returns true when opened but do not indent nor push into the
// ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label,
// ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen).
// You can remove the _NoTreePushOnOpen flag if you want behavior closer to
// normal TreeNode().
bool ImGui::CollapsingHeader(const char *label, ImGuiTreeNodeFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;
>>>>>>> master

  return TreeNodeBehavior(window->GetID(label),
                          flags | ImGuiTreeNodeFlags_CollapsingHeader |
                              ImGuiTreeNodeFlags_NoTreePushOnOpen,
                          label);
}

<<<<<<< HEAD
bool ImGui::TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g         = *GImGui;
    const char*   label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(str_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g         = *GImGui;
    const char*   label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(ptr_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeV(const char* str_id, const char* fmt, va_list args) { return TreeNodeExV(str_id, 0, fmt, args); }

bool ImGui::TreeNodeV(const void* ptr_id, const char* fmt, va_list args) { return TreeNodeExV(ptr_id, 0, fmt, args); }
=======
bool ImGui::CollapsingHeader(const char *label, bool *p_open,
                             ImGuiTreeNodeFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  if (p_open && !*p_open)
    return false;

  ImGuiID id = window->GetID(label);
  bool is_open = TreeNodeBehavior(
      id, flags | ImGuiTreeNodeFlags_CollapsingHeader |
              ImGuiTreeNodeFlags_NoTreePushOnOpen |
              (p_open ? ImGuiTreeNodeFlags_AllowOverlapMode : 0),
      label);
  if (p_open) {
    // Create a small overlapping close button // FIXME: We can evolve this into
    // user accessible helpers to add extra buttons on title bars, headers, etc.
    ImGuiContext &g = *GImGui;
    float button_sz = g.FontSize * 0.5f;
    if (CloseButton(window->GetID((void *)(intptr_t)(id + 1)),
                    ImVec2(ImMin(window->DC.LastItemRect.Max.x,
                                 window->ClipRect.Max.x) -
                               g.Style.FramePadding.x - button_sz,
                           window->DC.LastItemRect.Min.y +
                               g.Style.FramePadding.y + button_sz),
                    button_sz))
      *p_open = false;
  }

  return is_open;
}

bool ImGui::TreeNodeEx(const char *label, ImGuiTreeNodeFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  return TreeNodeBehavior(window->GetID(label), flags, label, NULL);
}

bool ImGui::TreeNodeExV(const char *str_id, ImGuiTreeNodeFlags flags,
                        const char *fmt, va_list args) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const char *label_end =
      g.TempBuffer +
      ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
  return TreeNodeBehavior(window->GetID(str_id), flags, g.TempBuffer,
                          label_end);
}

bool ImGui::TreeNodeExV(const void *ptr_id, ImGuiTreeNodeFlags flags,
                        const char *fmt, va_list args) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const char *label_end =
      g.TempBuffer +
      ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
  return TreeNodeBehavior(window->GetID(ptr_id), flags, g.TempBuffer,
                          label_end);
}
>>>>>>> master

bool ImGui::TreeNodeV(const char *str_id, const char *fmt, va_list args) {
  return TreeNodeExV(str_id, 0, fmt, args);
}

bool ImGui::TreeNodeV(const void *ptr_id, const char *fmt, va_list args) {
  return TreeNodeExV(ptr_id, 0, fmt, args);
}

bool ImGui::TreeNodeEx(const char *str_id, ImGuiTreeNodeFlags flags,
                       const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool is_open = TreeNodeExV(str_id, flags, fmt, args);
  va_end(args);
  return is_open;
}

bool ImGui::TreeNodeEx(const void *ptr_id, ImGuiTreeNodeFlags flags,
                       const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
  va_end(args);
  return is_open;
}

<<<<<<< HEAD
bool ImGui::TreeNode(const char* label)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;
    return TreeNodeBehavior(window->GetID(label), 0, label, NULL);
=======
bool ImGui::TreeNode(const char *str_id, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool is_open = TreeNodeExV(str_id, 0, fmt, args);
  va_end(args);
  return is_open;
>>>>>>> master
}

bool ImGui::TreeNode(const void *ptr_id, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
  va_end(args);
  return is_open;
}

bool ImGui::TreeNode(const char *label) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;
  return TreeNodeBehavior(window->GetID(label), 0, label, NULL);
}

void ImGui::TreeAdvanceToLabelPos() {
  ImGuiContext &g = *GImGui;
  g.CurrentWindow->DC.CursorPos.x += GetTreeNodeToLabelSpacing();
}

// Horizontal distance preceding label when using TreeNode() or Bullet()
float ImGui::GetTreeNodeToLabelSpacing() {
  ImGuiContext &g = *GImGui;
  return g.FontSize + (g.Style.FramePadding.x * 2.0f);
}

<<<<<<< HEAD
void ImGui::SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond)
{
    ImGuiContext& g           = *GImGui;
    g.SetNextTreeNodeOpenVal  = is_open;
    g.SetNextTreeNodeOpenCond = cond ? cond : ImGuiSetCond_Always;
=======
void ImGui::SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond) {
  ImGuiContext &g = *GImGui;
  g.SetNextTreeNodeOpenVal = is_open;
  g.SetNextTreeNodeOpenCond = cond ? cond : ImGuiSetCond_Always;
>>>>>>> master
}

void ImGui::PushID(const char *str_id) {
  ImGuiWindow *window = GetCurrentWindow();
  window->IDStack.push_back(window->GetID(str_id));
}

void ImGui::PushID(const char *str_id_begin, const char *str_id_end) {
  ImGuiWindow *window = GetCurrentWindow();
  window->IDStack.push_back(window->GetID(str_id_begin, str_id_end));
}

void ImGui::PushID(const void *ptr_id) {
  ImGuiWindow *window = GetCurrentWindow();
  window->IDStack.push_back(window->GetID(ptr_id));
}

<<<<<<< HEAD
void ImGui::PushID(int int_id)
{
    const void*  ptr_id = (void*)(intptr_t)int_id;
    ImGuiWindow* window = GetCurrentWindow();
    window->IDStack.push_back(window->GetID(ptr_id));
=======
void ImGui::PushID(int int_id) {
  const void *ptr_id = (void *)(intptr_t)int_id;
  ImGuiWindow *window = GetCurrentWindow();
  window->IDStack.push_back(window->GetID(ptr_id));
>>>>>>> master
}

void ImGui::PopID() {
  ImGuiWindow *window = GetCurrentWindow();
  window->IDStack.pop_back();
}

<<<<<<< HEAD
ImGuiID ImGui::GetID(const char* str_id) { return GImGui->CurrentWindow->GetID(str_id); }
=======
ImGuiID ImGui::GetID(const char *str_id) {
  return GImGui->CurrentWindow->GetID(str_id);
}
>>>>>>> master

ImGuiID ImGui::GetID(const char *str_id_begin, const char *str_id_end) {
  return GImGui->CurrentWindow->GetID(str_id_begin, str_id_end);
}

<<<<<<< HEAD
ImGuiID ImGui::GetID(const void* ptr_id) { return GImGui->CurrentWindow->GetID(ptr_id); }

void ImGui::Bullet()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float       line_height =
        ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y * 2), g.FontSize);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize, line_height));
    ItemSize(bb);
    if (!ItemAdd(bb, NULL)) {
        SameLine(0, style.FramePadding.x * 2);
        return;
    }

    // Render and stay on same line
    RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f, line_height * 0.5f));
    SameLine(0, style.FramePadding.x * 2);
}

// Text with a little bullet aligned to the typical tree node.
void ImGui::BulletTextV(const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const char*  text_begin         = g.TempBuffer;
    const char*  text_end           = text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    const ImVec2 label_size         = CalcTextSize(text_begin, text_end, false);
    const float  text_base_offset_y = ImMax(0.0f,
        window->DC.CurrentLineTextBaseOffset);  // Latch before ItemSize changes it
    const float line_height =
        ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y * 2), g.FontSize);
    const ImRect bb(window->DC.CursorPos,
        window->DC.CursorPos +
            ImVec2(g.FontSize + (label_size.x > 0.0f ? (label_size.x + style.FramePadding.x * 2) : 0.0f),
                ImMax(line_height,
                       label_size.y)));  // Empty text doesn't add padding
    ItemSize(bb);
    if (!ItemAdd(bb, NULL)) return;

    // Render
    RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f, line_height * 0.5f));
    RenderText(bb.Min + ImVec2(g.FontSize + style.FramePadding.x * 2, text_base_offset_y), text_begin, text_end, false);
=======
ImGuiID ImGui::GetID(const void *ptr_id) {
  return GImGui->CurrentWindow->GetID(ptr_id);
}

void ImGui::Bullet() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const float line_height =
      ImMax(ImMin(window->DC.CurrentLineHeight,
                  g.FontSize + g.Style.FramePadding.y * 2),
            g.FontSize);
  const ImRect bb(window->DC.CursorPos,
                  window->DC.CursorPos + ImVec2(g.FontSize, line_height));
  ItemSize(bb);
  if (!ItemAdd(bb, NULL)) {
    SameLine(0, style.FramePadding.x * 2);
    return;
  }

  // Render and stay on same line
  RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f,
                               line_height * 0.5f));
  SameLine(0, style.FramePadding.x * 2);
}

// Text with a little bullet aligned to the typical tree node.
void ImGui::BulletTextV(const char *fmt, va_list args) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  const char *text_begin = g.TempBuffer;
  const char *text_end =
      text_begin +
      ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
  const ImVec2 label_size = CalcTextSize(text_begin, text_end, false);
  const float text_base_offset_y = ImMax(
      0.0f,
      window->DC.CurrentLineTextBaseOffset); // Latch before ItemSize changes it
  const float line_height =
      ImMax(ImMin(window->DC.CurrentLineHeight,
                  g.FontSize + g.Style.FramePadding.y * 2),
            g.FontSize);
  const ImRect bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(g.FontSize + (label_size.x > 0.0f
                                   ? (label_size.x + style.FramePadding.x * 2)
                                   : 0.0f),
                 ImMax(line_height,
                       label_size.y))); // Empty text doesn't add padding
  ItemSize(bb);
  if (!ItemAdd(bb, NULL))
    return;

  // Render
  RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f,
                               line_height * 0.5f));
  RenderText(bb.Min + ImVec2(g.FontSize + style.FramePadding.x * 2,
                             text_base_offset_y),
             text_begin, text_end, false);
>>>>>>> master
}

void ImGui::BulletText(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  BulletTextV(fmt, args);
  va_end(args);
}

<<<<<<< HEAD
static inline void DataTypeFormatString(
    ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size)
{
    if (data_type == ImGuiDataType_Int)
        ImFormatString(buf, buf_size, display_format, *(int*)data_ptr);
    else if (data_type == ImGuiDataType_Float)
        ImFormatString(buf, buf_size, display_format, *(float*)data_ptr);
}

static inline void DataTypeFormatString(
    ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size)
{
    if (data_type == ImGuiDataType_Int) {
        if (decimal_precision < 0)
            ImFormatString(buf, buf_size, "%d", *(int*)data_ptr);
        else
            ImFormatString(buf, buf_size, "%.*d", decimal_precision, *(int*)data_ptr);
    }
    else if (data_type == ImGuiDataType_Float)
    {
        if (decimal_precision < 0)
            ImFormatString(buf, buf_size, "%f", *(float*)data_ptr);  // Ideally we'd have a minimum decimal
                                                                     // precision of 1 to visually denote
                                                                     // that it is a float, while hiding
                                                                     // non-significant digits?
        else
            ImFormatString(buf, buf_size, "%.*f", decimal_precision, *(float*)data_ptr);
    }
}

static void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2)  // Store into value1
{
    if (data_type == ImGuiDataType_Int) {
        if (op == '+')
            *(int*)value1 = *(int*)value1 + *(const int*)value2;
        else if (op == '-')
            *(int*)value1 = *(int*)value1 - *(const int*)value2;
    }
    else if (data_type == ImGuiDataType_Float)
    {
        if (op == '+')
            *(float*)value1 = *(float*)value1 + *(const float*)value2;
        else if (op == '-')
            *(float*)value1 = *(float*)value1 - *(const float*)value2;
    }
}

// User can input math operators (e.g. +100) to edit a numerical values.
static bool DataTypeApplyOpFromText(
    const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format)
{
    while (ImCharIsSpace(*buf)) buf++;

    // We don't support '-' op because it would conflict with inputing negative
    // value.
    // Instead you can use +-100 to subtract from an existing value
    char op = buf[0];
    if (op == '+' || op == '*' || op == '/') {
        buf++;
        while (ImCharIsSpace(*buf)) buf++;
    }
    else
    {
        op = 0;
    }
    if (!buf[0]) return false;

    if (data_type == ImGuiDataType_Int) {
        if (!scalar_format) scalar_format = "%d";
        int*      v                       = (int*)data_ptr;
        const int old_v                   = *v;
        int       arg0                    = *v;
        if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1) return false;

        // Store operand in a float so we can use fractional value for multipliers
        // (*1.1), but constant always parsed as integer so we can fit big integers
        // (e.g. 2000000003) past float precision
        float arg1 = 0.0f;
        if (op == '+') {
            if (sscanf(buf, "%f", &arg1) == 1) *v = (int)(arg0 + arg1);
        }  // Add (use "+-" to subtract)
        else if (op == '*')
        {
            if (sscanf(buf, "%f", &arg1) == 1) *v = (int)(arg0 * arg1);
        }  // Multiply
        else if (op == '/')
        {
            if (sscanf(buf, "%f", &arg1) == 1 && arg1 != 0.0f) *v = (int)(arg0 / arg1);
        }  // Divide
        else
        {
            if (sscanf(buf, scalar_format, &arg0) == 1) *v = arg0;
        }  // Assign constant
        return (old_v != *v);
    }
    else if (data_type == ImGuiDataType_Float)
    {
        // For floats we have to ignore format with precision (e.g. "%.2f") because
        // sscanf doesn't take them in
        scalar_format     = "%f";
        float*      v     = (float*)data_ptr;
        const float old_v = *v;
        float       arg0  = *v;
        if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1) return false;

        float arg1 = 0.0f;
        if (sscanf(buf, scalar_format, &arg1) < 1) return false;
        if (op == '+') {
            *v = arg0 + arg1;
        }  // Add (use "+-" to subtract)
        else if (op == '*')
        {
            *v = arg0 * arg1;
        }  // Multiply
        else if (op == '/')
        {
            if (arg1 != 0.0f) *v = arg0 / arg1;
        }  // Divide
        else
        {
            *v = arg1;
        }  // Assign constant
        return (old_v != *v);
    }

=======
static inline void DataTypeFormatString(ImGuiDataType data_type, void *data_ptr,
                                        const char *display_format, char *buf,
                                        int buf_size) {
  if (data_type == ImGuiDataType_Int)
    ImFormatString(buf, buf_size, display_format, *(int *)data_ptr);
  else if (data_type == ImGuiDataType_Float)
    ImFormatString(buf, buf_size, display_format, *(float *)data_ptr);
}

static inline void DataTypeFormatString(ImGuiDataType data_type, void *data_ptr,
                                        int decimal_precision, char *buf,
                                        int buf_size) {
  if (data_type == ImGuiDataType_Int) {
    if (decimal_precision < 0)
      ImFormatString(buf, buf_size, "%d", *(int *)data_ptr);
    else
      ImFormatString(buf, buf_size, "%.*d", decimal_precision,
                     *(int *)data_ptr);
  } else if (data_type == ImGuiDataType_Float) {
    if (decimal_precision < 0)
      ImFormatString(buf, buf_size, "%f",
                     *(float *)data_ptr); // Ideally we'd have a minimum decimal
                                          // precision of 1 to visually denote
                                          // that it is a float, while hiding
                                          // non-significant digits?
    else
      ImFormatString(buf, buf_size, "%.*f", decimal_precision,
                     *(float *)data_ptr);
  }
}

static void DataTypeApplyOp(ImGuiDataType data_type, int op, void *value1,
                            const void *value2) // Store into value1
{
  if (data_type == ImGuiDataType_Int) {
    if (op == '+')
      *(int *)value1 = *(int *)value1 + *(const int *)value2;
    else if (op == '-')
      *(int *)value1 = *(int *)value1 - *(const int *)value2;
  } else if (data_type == ImGuiDataType_Float) {
    if (op == '+')
      *(float *)value1 = *(float *)value1 + *(const float *)value2;
    else if (op == '-')
      *(float *)value1 = *(float *)value1 - *(const float *)value2;
  }
}

// User can input math operators (e.g. +100) to edit a numerical values.
static bool DataTypeApplyOpFromText(const char *buf,
                                    const char *initial_value_buf,
                                    ImGuiDataType data_type, void *data_ptr,
                                    const char *scalar_format) {
  while (ImCharIsSpace(*buf))
    buf++;

  // We don't support '-' op because it would conflict with inputing negative
  // value.
  // Instead you can use +-100 to subtract from an existing value
  char op = buf[0];
  if (op == '+' || op == '*' || op == '/') {
    buf++;
    while (ImCharIsSpace(*buf))
      buf++;
  } else {
    op = 0;
  }
  if (!buf[0])
>>>>>>> master
    return false;

  if (data_type == ImGuiDataType_Int) {
    if (!scalar_format)
      scalar_format = "%d";
    int *v = (int *)data_ptr;
    const int old_v = *v;
    int arg0 = *v;
    if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1)
      return false;

    // Store operand in a float so we can use fractional value for multipliers
    // (*1.1), but constant always parsed as integer so we can fit big integers
    // (e.g. 2000000003) past float precision
    float arg1 = 0.0f;
    if (op == '+') {
      if (sscanf(buf, "%f", &arg1) == 1)
        *v = (int)(arg0 + arg1);
    } // Add (use "+-" to subtract)
    else if (op == '*') {
      if (sscanf(buf, "%f", &arg1) == 1)
        *v = (int)(arg0 * arg1);
    } // Multiply
    else if (op == '/') {
      if (sscanf(buf, "%f", &arg1) == 1 && arg1 != 0.0f)
        *v = (int)(arg0 / arg1);
    } // Divide
    else {
      if (sscanf(buf, scalar_format, &arg0) == 1)
        *v = arg0;
    } // Assign constant
    return (old_v != *v);
  } else if (data_type == ImGuiDataType_Float) {
    // For floats we have to ignore format with precision (e.g. "%.2f") because
    // sscanf doesn't take them in
    scalar_format = "%f";
    float *v = (float *)data_ptr;
    const float old_v = *v;
    float arg0 = *v;
    if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1)
      return false;

    float arg1 = 0.0f;
    if (sscanf(buf, scalar_format, &arg1) < 1)
      return false;
    if (op == '+') {
      *v = arg0 + arg1;
    } // Add (use "+-" to subtract)
    else if (op == '*') {
      *v = arg0 * arg1;
    } // Multiply
    else if (op == '/') {
      if (arg1 != 0.0f)
        *v = arg0 / arg1;
    } // Divide
    else {
      *v = arg1;
    } // Assign constant
    return (old_v != *v);
  }

  return false;
}

// Create text input in place of a slider (when CTRL+Clicking on slider)
<<<<<<< HEAD
bool ImGui::InputScalarAsWidgetReplacement(
    const ImRect& aabb, const char* label, ImGuiDataType data_type, void* data_ptr, ImGuiID id, int decimal_precision)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    // Our replacement widget will override the focus ID (registered previously to
    // allow for a TAB focus to happen)
    SetActiveID(g.ScalarAsInputTextId, window);
    SetHoveredID(0);
    FocusableItemUnregister(window);

    char buf[32];
    DataTypeFormatString(data_type, data_ptr, decimal_precision, buf, IM_ARRAYSIZE(buf));
    bool text_value_changed = InputTextEx(label,
        buf,
        IM_ARRAYSIZE(buf),
        aabb.GetSize(),
        ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll);
    if (g.ScalarAsInputTextId == 0) {
        // First frame
        IM_ASSERT(g.ActiveId == id);  // InputText ID expected to match the Slider ID
                                      // (else we'd need to store them both, which is
                                      // also possible)
        g.ScalarAsInputTextId = g.ActiveId;
        SetHoveredID(id);
    }
    else if (g.ActiveId != g.ScalarAsInputTextId)
    {
        // Release
        g.ScalarAsInputTextId = 0;
    }
    if (text_value_changed)
        return DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, NULL);
    return false;
}

// Parse display precision back from the display format string
int ImGui::ParseFormatPrecision(const char* fmt, int default_precision)
{
    int precision = default_precision;
    while ((fmt = strchr(fmt, '%')) != NULL) {
        fmt++;
        if (fmt[0] == '%') {
            fmt++;
            continue;
        }  // Ignore "%%"
        while (*fmt >= '0' && *fmt <= '9') fmt++;
        if (*fmt == '.') {
            precision                                      = atoi(fmt + 1);
            if (precision < 0 || precision > 10) precision = default_precision;
        }
        break;
=======
bool ImGui::InputScalarAsWidgetReplacement(const ImRect &aabb,
                                           const char *label,
                                           ImGuiDataType data_type,
                                           void *data_ptr, ImGuiID id,
                                           int decimal_precision) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  // Our replacement widget will override the focus ID (registered previously to
  // allow for a TAB focus to happen)
  SetActiveID(g.ScalarAsInputTextId, window);
  SetHoveredID(0);
  FocusableItemUnregister(window);

  char buf[32];
  DataTypeFormatString(data_type, data_ptr, decimal_precision, buf,
                       IM_ARRAYSIZE(buf));
  bool text_value_changed = InputTextEx(
      label, buf, IM_ARRAYSIZE(buf), aabb.GetSize(),
      ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll);
  if (g.ScalarAsInputTextId == 0) {
    // First frame
    IM_ASSERT(g.ActiveId == id); // InputText ID expected to match the Slider ID
                                 // (else we'd need to store them both, which is
                                 // also possible)
    g.ScalarAsInputTextId = g.ActiveId;
    SetHoveredID(id);
  } else if (g.ActiveId != g.ScalarAsInputTextId) {
    // Release
    g.ScalarAsInputTextId = 0;
  }
  if (text_value_changed)
    return DataTypeApplyOpFromText(buf,
                                   GImGui->InputTextState.InitialText.begin(),
                                   data_type, data_ptr, NULL);
  return false;
}

// Parse display precision back from the display format string
int ImGui::ParseFormatPrecision(const char *fmt, int default_precision) {
  int precision = default_precision;
  while ((fmt = strchr(fmt, '%')) != NULL) {
    fmt++;
    if (fmt[0] == '%') {
      fmt++;
      continue;
    } // Ignore "%%"
    while (*fmt >= '0' && *fmt <= '9')
      fmt++;
    if (*fmt == '.') {
      precision = atoi(fmt + 1);
      if (precision < 0 || precision > 10)
        precision = default_precision;
>>>>>>> master
    }
    break;
  }
  return precision;
}

<<<<<<< HEAD
float ImGui::RoundScalar(float value, int decimal_precision)
{
    // Round past decimal precision
    // So when our value is 1.99999 with a precision of 0.001 we'll end up
    // rounding to 2.0
    // FIXME: Investigate better rounding methods
    static const float min_steps[10] = {
        1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f};
    float min_step = (decimal_precision >= 0 && decimal_precision < 10) ? min_steps[decimal_precision]
                                                                        : powf(10.0f, (float)-decimal_precision);
    bool negative   = value < 0.0f;
    value           = fabsf(value);
    float remainder = fmodf(value, min_step);
    if (remainder <= min_step * 0.5f)
        value -= remainder;
    else
        value += (min_step - remainder);
    return negative ? -value : value;
}

static inline float SliderBehaviorCalcRatioFromValue(
    float v, float v_min, float v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max) return 0.0f;

    const bool  is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
    const float v_clamped     = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
    if (is_non_linear) {
        if (v_clamped < 0.0f) {
            const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
            return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
        }
        else
        {
            const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
            return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
        }
=======
float ImGui::RoundScalar(float value, int decimal_precision) {
  // Round past decimal precision
  // So when our value is 1.99999 with a precision of 0.001 we'll end up
  // rounding to 2.0
  // FIXME: Investigate better rounding methods
  static const float min_steps[10] = {
      1.0f,     0.1f,      0.01f,      0.001f,      0.0001f,
      0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f};
  float min_step = (decimal_precision >= 0 && decimal_precision < 10)
                       ? min_steps[decimal_precision]
                       : powf(10.0f, (float)-decimal_precision);
  bool negative = value < 0.0f;
  value = fabsf(value);
  float remainder = fmodf(value, min_step);
  if (remainder <= min_step * 0.5f)
    value -= remainder;
  else
    value += (min_step - remainder);
  return negative ? -value : value;
}

static inline float SliderBehaviorCalcRatioFromValue(float v, float v_min,
                                                     float v_max, float power,
                                                     float linear_zero_pos) {
  if (v_min == v_max)
    return 0.0f;

  const bool is_non_linear =
      (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
  const float v_clamped =
      (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
  if (is_non_linear) {
    if (v_clamped < 0.0f) {
      const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
      return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
    } else {
      const float f =
          (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
      return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
>>>>>>> master
    }
  }

  // Linear slider
  return (v_clamped - v_min) / (v_max - v_min);
}

<<<<<<< HEAD
bool ImGui::SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power,
    int decimal_precision, ImGuiSliderFlags flags)
{
    ImGuiContext&     g      = *GImGui;
    ImGuiWindow*      window = GetCurrentWindow();
    const ImGuiStyle& style  = g.Style;
=======
bool ImGui::SliderBehavior(const ImRect &frame_bb, ImGuiID id, float *v,
                           float v_min, float v_max, float power,
                           int decimal_precision, ImGuiSliderFlags flags) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();
  const ImGuiStyle &style = g.Style;
>>>>>>> master

  // Draw frame
  RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true,
              style.FrameRounding);

<<<<<<< HEAD
    const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
    const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

    const float grab_padding = 2.0f;
    const float slider_sz =
        is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
    float grab_sz;
    if (decimal_precision > 0)
        grab_sz = ImMin(style.GrabMinSize, slider_sz);
    else
        grab_sz = ImMin(
            ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize),
            slider_sz);  // Integer sliders, if possible have the grab size represent
                         // 1 unit
    const float slider_usable_sz = slider_sz - grab_sz;
    const float slider_usable_pos_min =
        (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz * 0.5f;
    const float slider_usable_pos_max =
        (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz * 0.5f;

    // For logarithmic sliders that cross over sign boundary we want the
    // exponential increase to be symmetric around 0.0f
    float linear_zero_pos = 0.0f;  // 0.0->1.0f
    if (v_min * v_max < 0.0f) {
        // Different sign
        const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
        const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
        linear_zero_pos                  = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
    }
    else
    {
        // Same sign
        linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
    }

    // Process clicking on the slider
    bool value_changed = false;
    if (g.ActiveId == id) {
        if (g.IO.MouseDown[0]) {
            const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
            float       clicked_t     = (slider_usable_sz > 0.0f)
                                  ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f)
                                  : 0.0f;
            if (!is_horizontal) clicked_t = 1.0f - clicked_t;

            float new_value;
            if (is_non_linear) {
                // Account for logarithmic scale on both sides of the zero
                if (clicked_t < linear_zero_pos) {
                    // Negative: rescale to the negative range before powering
                    float a   = 1.0f - (clicked_t / linear_zero_pos);
                    a         = powf(a, power);
                    new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
                }
                else
                {
                    // Positive: rescale to the positive range before powering
                    float a;
                    if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
                        a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
                    else
                        a     = clicked_t;
                    a         = powf(a, power);
                    new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
                }
            }
            else
            {
                // Linear slider
                new_value = ImLerp(v_min, v_max, clicked_t);
            }

            // Round past decimal precision
            new_value = RoundScalar(new_value, decimal_precision);
            if (*v != new_value) {
                *v            = new_value;
                value_changed = true;
            }
        }
        else
        {
            ClearActiveID();
=======
  const bool is_non_linear =
      (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
  const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

  const float grab_padding = 2.0f;
  const float slider_sz = is_horizontal
                              ? (frame_bb.GetWidth() - grab_padding * 2.0f)
                              : (frame_bb.GetHeight() - grab_padding * 2.0f);
  float grab_sz;
  if (decimal_precision > 0)
    grab_sz = ImMin(style.GrabMinSize, slider_sz);
  else
    grab_sz = ImMin(
        ImMax(1.0f * (slider_sz /
                      ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)),
              style.GrabMinSize),
        slider_sz); // Integer sliders, if possible have the grab size represent
                    // 1 unit
  const float slider_usable_sz = slider_sz - grab_sz;
  const float slider_usable_pos_min =
      (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding +
      grab_sz * 0.5f;
  const float slider_usable_pos_max =
      (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding -
      grab_sz * 0.5f;

  // For logarithmic sliders that cross over sign boundary we want the
  // exponential increase to be symmetric around 0.0f
  float linear_zero_pos = 0.0f; // 0.0->1.0f
  if (v_min * v_max < 0.0f) {
    // Different sign
    const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
    const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
    linear_zero_pos =
        linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
  } else {
    // Same sign
    linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
  }

  // Process clicking on the slider
  bool value_changed = false;
  if (g.ActiveId == id) {
    if (g.IO.MouseDown[0]) {
      const float mouse_abs_pos =
          is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
      float clicked_t = (slider_usable_sz > 0.0f)
                            ? ImClamp((mouse_abs_pos - slider_usable_pos_min) /
                                          slider_usable_sz,
                                      0.0f, 1.0f)
                            : 0.0f;
      if (!is_horizontal)
        clicked_t = 1.0f - clicked_t;

      float new_value;
      if (is_non_linear) {
        // Account for logarithmic scale on both sides of the zero
        if (clicked_t < linear_zero_pos) {
          // Negative: rescale to the negative range before powering
          float a = 1.0f - (clicked_t / linear_zero_pos);
          a = powf(a, power);
          new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
        } else {
          // Positive: rescale to the positive range before powering
          float a;
          if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
            a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
          else
            a = clicked_t;
          a = powf(a, power);
          new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
>>>>>>> master
        }
      } else {
        // Linear slider
        new_value = ImLerp(v_min, v_max, clicked_t);
      }

      // Round past decimal precision
      new_value = RoundScalar(new_value, decimal_precision);
      if (*v != new_value) {
        *v = new_value;
        value_changed = true;
      }
    } else {
      ClearActiveID();
    }
  }

  // Calculate slider grab positioning
  float grab_t = SliderBehaviorCalcRatioFromValue(*v, v_min, v_max, power,
                                                  linear_zero_pos);

<<<<<<< HEAD
    // Draw
    if (!is_horizontal) grab_t = 1.0f - grab_t;
    const float grab_pos       = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
    ImRect      grab_bb;
    if (is_horizontal)
        grab_bb = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding),
            ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
    else
        grab_bb = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f),
            ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
    window->DrawList->AddRectFilled(grab_bb.Min,
        grab_bb.Max,
        GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab),
        style.GrabRounding);
=======
  // Draw
  if (!is_horizontal)
    grab_t = 1.0f - grab_t;
  const float grab_pos =
      ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
  ImRect grab_bb;
  if (is_horizontal)
    grab_bb = ImRect(
        ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding),
        ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
  else
    grab_bb = ImRect(
        ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f),
        ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
  window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max,
                                  GetColorU32(g.ActiveId == id
                                                  ? ImGuiCol_SliderGrabActive
                                                  : ImGuiCol_SliderGrab),
                                  style.GrabRounding);
>>>>>>> master

  return value_changed;
}

// Use power!=1.0 for logarithmic sliders.
// Adjust display_format to decorate the value with a prefix or a suffix.
//   "%.3f"         1.234
//   "%5.2f secs"   01.23 secs
//   "Gold: %.0f"   Gold: 1
<<<<<<< HEAD
bool ImGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);
    const float       w     = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(
        window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    // NB- we don't call ItemSize() yet because we may turn into a text edit box
    // below
    if (!ItemAdd(total_bb, &id)) {
        ItemSize(total_bb, style.FramePadding.y);
        return false;
    }

    const bool hovered = IsHovered(frame_bb, id);
    if (hovered) SetHoveredID(id);

    if (!display_format) display_format = "%.3f";
    int decimal_precision               = ParseFormatPrecision(display_format, 3);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    bool       start_text_input    = false;
    const bool tab_focus_requested = FocusableItemRegister(window, g.ActiveId == id);
    if (tab_focus_requested || (hovered && g.IO.MouseClicked[0])) {
        SetActiveID(id, window);
        FocusWindow(window);

        if (tab_focus_requested || g.IO.KeyCtrl) {
            start_text_input      = true;
            g.ScalarAsInputTextId = 0;
        }
    }
    if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
        return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);
=======
bool ImGui::SliderFloat(const char *label, float *v, float v_min, float v_max,
                        const char *display_format, float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w = CalcItemWidth();

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect frame_bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max +
                            ImVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                                   0.0f));
>>>>>>> master

  // NB- we don't call ItemSize() yet because we may turn into a text edit box
  // below
  if (!ItemAdd(total_bb, &id)) {
    ItemSize(total_bb, style.FramePadding.y);
    return false;
  }

  const bool hovered = IsHovered(frame_bb, id);
  if (hovered)
    SetHoveredID(id);

<<<<<<< HEAD
    // Display value using user-provided display format so user can add
    // prefix/suffix/decorations to the value.
    char        value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));
=======
  if (!display_format)
    display_format = "%.3f";
  int decimal_precision = ParseFormatPrecision(display_format, 3);
>>>>>>> master

  // Tabbing or CTRL-clicking on Slider turns it into an input box
  bool start_text_input = false;
  const bool tab_focus_requested =
      FocusableItemRegister(window, g.ActiveId == id);
  if (tab_focus_requested || (hovered && g.IO.MouseClicked[0])) {
    SetActiveID(id, window);
    FocusWindow(window);

<<<<<<< HEAD
    return value_changed;
}

bool ImGui::VSliderFloat(
    const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, &id)) return false;

    const bool hovered = IsHovered(frame_bb, id);
    if (hovered) SetHoveredID(id);

    if (!display_format) display_format = "%.3f";
    int decimal_precision               = ParseFormatPrecision(display_format, 3);

    if (hovered && g.IO.MouseClicked[0]) {
        SetActiveID(id, window);
        FocusWindow(window);
=======
    if (tab_focus_requested || g.IO.KeyCtrl) {
      start_text_input = true;
      g.ScalarAsInputTextId = 0;
>>>>>>> master
    }
  }
  if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
    return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float,
                                          v, id, decimal_precision);

<<<<<<< HEAD
    // Actual slider behavior + render grab
    bool value_changed =
        SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision, ImGuiSliderFlags_Vertical);

    // Display value using user-provided display format so user can add
    // prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char  value_buf[64];
    char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
        frame_bb.Max,
        value_buf,
        value_buf_end,
        NULL,
        ImVec2(0.5f, 0.0f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
=======
  ItemSize(total_bb, style.FramePadding.y);

  // Actual slider behavior + render grab
  const bool value_changed =
      SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision);
>>>>>>> master

  // Display value using user-provided display format so user can add
  // prefix/suffix/decorations to the value.
  char value_buf[64];
  const char *value_buf_end =
      value_buf +
      ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
  RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL,
                    ImVec2(0.5f, 0.5f));

  if (label_size.x > 0.0f)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                      frame_bb.Min.y + style.FramePadding.y),
               label);

  return value_changed;
}

<<<<<<< HEAD
bool ImGui::SliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max)
{
    float v_deg         = (*v_rad) * 360.0f / (2 * IM_PI);
    bool  value_changed = SliderFloat(label, &v_deg, v_degrees_min, v_degrees_max, "%.0f deg", 1.0f);
    *v_rad              = v_deg * (2 * IM_PI) / 360.0f;
    return value_changed;
}

bool ImGui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format)
{
    if (!display_format) display_format = "%.0f";
    float v_f                           = (float)*v;
    bool  value_changed                 = SliderFloat(label, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
    *v                                  = (int)v_f;
    return value_changed;
}

bool ImGui::VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format)
{
    if (!display_format) display_format = "%.0f";
    float v_f                           = (float)*v;
    bool  value_changed = VSliderFloat(label, size, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
    *v                  = (int)v_f;
    return value_changed;
}

// Add multiple sliders on 1 line for compact edition of multiple components
bool ImGui::SliderFloatN(
    const char* label, float* v, int components, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= SliderFloat("##v", &v[i], v_min, v_max, display_format, power);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
=======
bool ImGui::VSliderFloat(const char *label, const ImVec2 &size, float *v,
                         float v_min, float v_max, const char *display_format,
                         float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
  const ImRect bb(frame_bb.Min,
                  frame_bb.Max +
                      ImVec2(label_size.x > 0.0f
                                 ? style.ItemInnerSpacing.x + label_size.x
                                 : 0.0f,
                             0.0f));

  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(frame_bb, &id))
    return false;

  const bool hovered = IsHovered(frame_bb, id);
  if (hovered)
    SetHoveredID(id);

  if (!display_format)
    display_format = "%.3f";
  int decimal_precision = ParseFormatPrecision(display_format, 3);

  if (hovered && g.IO.MouseClicked[0]) {
    SetActiveID(id, window);
    FocusWindow(window);
  }

  // Actual slider behavior + render grab
  bool value_changed =
      SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision,
                     ImGuiSliderFlags_Vertical);

  // Display value using user-provided display format so user can add
  // prefix/suffix/decorations to the value.
  // For the vertical slider we allow centered text to overlap the frame padding
  char value_buf[64];
  char *value_buf_end =
      value_buf +
      ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
  RenderTextClipped(
      ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
      frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.0f));
  if (label_size.x > 0.0f)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                      frame_bb.Min.y + style.FramePadding.y),
               label);

  return value_changed;
}

bool ImGui::SliderAngle(const char *label, float *v_rad, float v_degrees_min,
                        float v_degrees_max) {
  float v_deg = (*v_rad) * 360.0f / (2 * IM_PI);
  bool value_changed = SliderFloat(label, &v_deg, v_degrees_min, v_degrees_max,
                                   "%.0f deg", 1.0f);
  *v_rad = v_deg * (2 * IM_PI) / 360.0f;
  return value_changed;
}

bool ImGui::SliderInt(const char *label, int *v, int v_min, int v_max,
                      const char *display_format) {
  if (!display_format)
    display_format = "%.0f";
  float v_f = (float)*v;
  bool value_changed = SliderFloat(label, &v_f, (float)v_min, (float)v_max,
                                   display_format, 1.0f);
  *v = (int)v_f;
  return value_changed;
}

bool ImGui::VSliderInt(const char *label, const ImVec2 &size, int *v, int v_min,
                       int v_max, const char *display_format) {
  if (!display_format)
    display_format = "%.0f";
  float v_f = (float)*v;
  bool value_changed = VSliderFloat(label, size, &v_f, (float)v_min,
                                    (float)v_max, display_format, 1.0f);
  *v = (int)v_f;
  return value_changed;
}

// Add multiple sliders on 1 line for compact edition of multiple components
bool ImGui::SliderFloatN(const char *label, float *v, int components,
                         float v_min, float v_max, const char *display_format,
                         float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |=
        SliderFloat("##v", &v[i], v_min, v_max, display_format, power);
    SameLine(0, g.Style.ItemInnerSpacing.x);
>>>>>>> master
    PopID();
    PopItemWidth();
  }
  PopID();

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

<<<<<<< HEAD
bool ImGui::SliderFloat2(
    const char* label, float v[2], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 2, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat3(
    const char* label, float v[3], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 3, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat4(
    const char* label, float v[4], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 4, v_min, v_max, display_format, power);
}

bool ImGui::SliderIntN(const char* label, int* v, int components, int v_min, int v_max, const char* display_format)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= SliderInt("##v", &v[i], v_min, v_max, display_format);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
=======
bool ImGui::SliderFloat2(const char *label, float v[2], float v_min,
                         float v_max, const char *display_format, float power) {
  return SliderFloatN(label, v, 2, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat3(const char *label, float v[3], float v_min,
                         float v_max, const char *display_format, float power) {
  return SliderFloatN(label, v, 3, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat4(const char *label, float v[4], float v_min,
                         float v_max, const char *display_format, float power) {
  return SliderFloatN(label, v, 4, v_min, v_max, display_format, power);
}

bool ImGui::SliderIntN(const char *label, int *v, int components, int v_min,
                       int v_max, const char *display_format) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |= SliderInt("##v", &v[i], v_min, v_max, display_format);
    SameLine(0, g.Style.ItemInnerSpacing.x);
>>>>>>> master
    PopID();
    PopItemWidth();
  }
  PopID();

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

bool ImGui::SliderInt2(const char *label, int v[2], int v_min, int v_max,
                       const char *display_format) {
  return SliderIntN(label, v, 2, v_min, v_max, display_format);
}

bool ImGui::SliderInt3(const char *label, int v[3], int v_min, int v_max,
                       const char *display_format) {
  return SliderIntN(label, v, 3, v_min, v_max, display_format);
}

bool ImGui::SliderInt4(const char *label, int v[4], int v_min, int v_max,
                       const char *display_format) {
  return SliderIntN(label, v, 4, v_min, v_max, display_format);
}

<<<<<<< HEAD
bool ImGui::DragBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_speed, float v_min, float v_max,
    int decimal_precision, float power)
{
    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Draw frame
    const ImU32 frame_col = GetColorU32(
        g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);
=======
bool ImGui::DragBehavior(const ImRect &frame_bb, ImGuiID id, float *v,
                         float v_speed, float v_min, float v_max,
                         int decimal_precision, float power) {
  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  // Draw frame
  const ImU32 frame_col =
      GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive
                                   : g.HoveredId == id ? ImGuiCol_FrameBgHovered
                                                       : ImGuiCol_FrameBg);
  RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);
>>>>>>> master

  bool value_changed = false;

<<<<<<< HEAD
    // Process clicking on the drag
    if (g.ActiveId == id) {
        if (g.IO.MouseDown[0]) {
            if (g.ActiveIdIsJustActivated) {
                // Lock current value on click
                g.DragCurrentValue   = *v;
                g.DragLastMouseDelta = ImVec2(0.f, 0.f);
            }

            float        v_cur            = g.DragCurrentValue;
            const ImVec2 mouse_drag_delta = GetMouseDragDelta(0, 1.0f);
            if (fabsf(mouse_drag_delta.x - g.DragLastMouseDelta.x) > 0.0f) {
                float speed = v_speed;
                if (speed == 0.0f && (v_max - v_min) != 0.0f && (v_max - v_min) < FLT_MAX)
                    speed                                                = (v_max - v_min) * g.DragSpeedDefaultRatio;
                if (g.IO.KeyShift && g.DragSpeedScaleFast >= 0.0f) speed = speed * g.DragSpeedScaleFast;
                if (g.IO.KeyAlt && g.DragSpeedScaleSlow >= 0.0f) speed   = speed * g.DragSpeedScaleSlow;

                float delta = (mouse_drag_delta.x - g.DragLastMouseDelta.x) * speed;
                if (fabsf(power - 1.0f) > 0.001f) {
                    // Logarithmic curve on both side of 0.0
                    float v0_abs  = v_cur >= 0.0f ? v_cur : -v_cur;
                    float v0_sign = v_cur >= 0.0f ? 1.0f : -1.0f;
                    float v1      = powf(v0_abs, 1.0f / power) + (delta * v0_sign);
                    float v1_abs  = v1 >= 0.0f ? v1 : -v1;
                    float v1_sign = v1 >= 0.0f ? 1.0f : -1.0f;                // Crossed sign line
                    v_cur         = powf(v1_abs, power) * v0_sign * v1_sign;  // Reapply sign
                }
                else
                {
                    v_cur += delta;
                }
                g.DragLastMouseDelta.x = mouse_drag_delta.x;

                // Clamp
                if (v_min < v_max) v_cur = ImClamp(v_cur, v_min, v_max);
                g.DragCurrentValue       = v_cur;
            }

            // Round to user desired precision, then apply
            v_cur = RoundScalar(v_cur, decimal_precision);
            if (*v != v_cur) {
                *v            = v_cur;
                value_changed = true;
            }
=======
  // Process clicking on the drag
  if (g.ActiveId == id) {
    if (g.IO.MouseDown[0]) {
      if (g.ActiveIdIsJustActivated) {
        // Lock current value on click
        g.DragCurrentValue = *v;
        g.DragLastMouseDelta = ImVec2(0.f, 0.f);
      }

      float v_cur = g.DragCurrentValue;
      const ImVec2 mouse_drag_delta = GetMouseDragDelta(0, 1.0f);
      if (fabsf(mouse_drag_delta.x - g.DragLastMouseDelta.x) > 0.0f) {
        float speed = v_speed;
        if (speed == 0.0f && (v_max - v_min) != 0.0f &&
            (v_max - v_min) < FLT_MAX)
          speed = (v_max - v_min) * g.DragSpeedDefaultRatio;
        if (g.IO.KeyShift && g.DragSpeedScaleFast >= 0.0f)
          speed = speed * g.DragSpeedScaleFast;
        if (g.IO.KeyAlt && g.DragSpeedScaleSlow >= 0.0f)
          speed = speed * g.DragSpeedScaleSlow;

        float delta = (mouse_drag_delta.x - g.DragLastMouseDelta.x) * speed;
        if (fabsf(power - 1.0f) > 0.001f) {
          // Logarithmic curve on both side of 0.0
          float v0_abs = v_cur >= 0.0f ? v_cur : -v_cur;
          float v0_sign = v_cur >= 0.0f ? 1.0f : -1.0f;
          float v1 = powf(v0_abs, 1.0f / power) + (delta * v0_sign);
          float v1_abs = v1 >= 0.0f ? v1 : -v1;
          float v1_sign = v1 >= 0.0f ? 1.0f : -1.0f;       // Crossed sign line
          v_cur = powf(v1_abs, power) * v0_sign * v1_sign; // Reapply sign
        } else {
          v_cur += delta;
>>>>>>> master
        }
        g.DragLastMouseDelta.x = mouse_drag_delta.x;

        // Clamp
        if (v_min < v_max)
          v_cur = ImClamp(v_cur, v_min, v_max);
        g.DragCurrentValue = v_cur;
      }

      // Round to user desired precision, then apply
      v_cur = RoundScalar(v_cur, decimal_precision);
      if (*v != v_cur) {
        *v = v_cur;
        value_changed = true;
      }
    } else {
      ClearActiveID();
    }
  }

  return value_changed;
}

<<<<<<< HEAD
bool ImGui::DragFloat(
    const char* label, float* v, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);
    const float       w     = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(
        window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    // NB- we don't call ItemSize() yet because we may turn into a text edit box
    // below
    if (!ItemAdd(total_bb, &id)) {
        ItemSize(total_bb, style.FramePadding.y);
        return false;
    }

    const bool hovered = IsHovered(frame_bb, id);
    if (hovered) SetHoveredID(id);

    if (!display_format) display_format = "%.3f";
    int decimal_precision               = ParseFormatPrecision(display_format, 3);

    // Tabbing or CTRL-clicking on Drag turns it into an input box
    bool       start_text_input    = false;
    const bool tab_focus_requested = FocusableItemRegister(window, g.ActiveId == id);
    if (tab_focus_requested || (hovered && (g.IO.MouseClicked[0] | g.IO.MouseDoubleClicked[0]))) {
        SetActiveID(id, window);
        FocusWindow(window);

        if (tab_focus_requested || g.IO.KeyCtrl || g.IO.MouseDoubleClicked[0]) {
            start_text_input      = true;
            g.ScalarAsInputTextId = 0;
        }
    }
    if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
        return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);

    // Actual drag behavior
=======
bool ImGui::DragFloat(const char *label, float *v, float v_speed, float v_min,
                      float v_max, const char *display_format, float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w = CalcItemWidth();

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect frame_bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
  const ImRect inner_bb(frame_bb.Min + style.FramePadding,
                        frame_bb.Max - style.FramePadding);
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max +
                            ImVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                                   0.0f));

  // NB- we don't call ItemSize() yet because we may turn into a text edit box
  // below
  if (!ItemAdd(total_bb, &id)) {
>>>>>>> master
    ItemSize(total_bb, style.FramePadding.y);
    return false;
  }

<<<<<<< HEAD
    // Display value using user-provided display format so user can add
    // prefix/suffix/decorations to the value.
    char        value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f) RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
=======
  const bool hovered = IsHovered(frame_bb, id);
  if (hovered)
    SetHoveredID(id);

  if (!display_format)
    display_format = "%.3f";
  int decimal_precision = ParseFormatPrecision(display_format, 3);
>>>>>>> master

  // Tabbing or CTRL-clicking on Drag turns it into an input box
  bool start_text_input = false;
  const bool tab_focus_requested =
      FocusableItemRegister(window, g.ActiveId == id);
  if (tab_focus_requested ||
      (hovered && (g.IO.MouseClicked[0] | g.IO.MouseDoubleClicked[0]))) {
    SetActiveID(id, window);
    FocusWindow(window);

<<<<<<< HEAD
bool ImGui::DragFloatN(const char* label, float* v, int components, float v_speed, float v_min, float v_max,
    const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
=======
    if (tab_focus_requested || g.IO.KeyCtrl || g.IO.MouseDoubleClicked[0]) {
      start_text_input = true;
      g.ScalarAsInputTextId = 0;
>>>>>>> master
    }
  }
  if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
    return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float,
                                          v, id, decimal_precision);

  // Actual drag behavior
  ItemSize(total_bb, style.FramePadding.y);
  const bool value_changed = DragBehavior(frame_bb, id, v, v_speed, v_min,
                                          v_max, decimal_precision, power);

  // Display value using user-provided display format so user can add
  // prefix/suffix/decorations to the value.
  char value_buf[64];
  const char *value_buf_end =
      value_buf +
      ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
  RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL,
                    ImVec2(0.5f, 0.5f));

  if (label_size.x > 0.0f)
    RenderText(
        ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y),
        label);

  return value_changed;
}

<<<<<<< HEAD
bool ImGui::DragFloat2(
    const char* label, float v[2], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 2, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat3(
    const char* label, float v[3], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 3, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat4(
    const char* label, float v[4], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 4, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min,
    float v_max, const char* display_format, const char* display_format_max, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2);

    bool value_changed = DragFloat("##min",
        v_current_min,
        v_speed,
        (v_min >= v_max) ? -FLT_MAX : v_min,
        (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max),
        display_format,
        power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragFloat("##max",
        v_current_max,
        v_speed,
        (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min),
        (v_min >= v_max) ? FLT_MAX : v_max,
        display_format_max ? display_format_max : display_format,
        power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();
=======
bool ImGui::DragFloatN(const char *label, float *v, int components,
                       float v_speed, float v_min, float v_max,
                       const char *display_format, float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |=
        DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);
    SameLine(0, g.Style.ItemInnerSpacing.x);
>>>>>>> master
    PopID();
    PopItemWidth();
  }
  PopID();

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

bool ImGui::DragFloat2(const char *label, float v[2], float v_speed,
                       float v_min, float v_max, const char *display_format,
                       float power) {
  return DragFloatN(label, v, 2, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat3(const char *label, float v[3], float v_speed,
                       float v_min, float v_max, const char *display_format,
                       float power) {
  return DragFloatN(label, v, 3, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat4(const char *label, float v[4], float v_speed,
                       float v_min, float v_max, const char *display_format,
                       float power) {
  return DragFloatN(label, v, 4, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloatRange2(const char *label, float *v_current_min,
                            float *v_current_max, float v_speed, float v_min,
                            float v_max, const char *display_format,
                            const char *display_format_max, float power) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  PushID(label);
  BeginGroup();
  PushMultiItemsWidths(2);

  bool value_changed = DragFloat(
      "##min", v_current_min, v_speed, (v_min >= v_max) ? -FLT_MAX : v_min,
      (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max),
      display_format, power);
  PopItemWidth();
  SameLine(0, g.Style.ItemInnerSpacing.x);
  value_changed |= DragFloat(
      "##max", v_current_max, v_speed,
      (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min),
      (v_min >= v_max) ? FLT_MAX : v_max,
      display_format_max ? display_format_max : display_format, power);
  PopItemWidth();
  SameLine(0, g.Style.ItemInnerSpacing.x);

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();
  PopID();

  return value_changed;
}

// NB: v_speed is float to allow adjusting the drag speed with more precision
<<<<<<< HEAD
bool ImGui::DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* display_format)
{
    if (!display_format) display_format = "%.0f";
    float v_f                           = (float)*v;
    bool  value_changed                 = DragFloat(label, &v_f, v_speed, (float)v_min, (float)v_max, display_format);
    *v                                  = (int)v_f;
    return value_changed;
}

bool ImGui::DragIntN(
    const char* label, int* v, int components, float v_speed, int v_min, int v_max, const char* display_format)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= DragInt("##v", &v[i], v_speed, v_min, v_max, display_format);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 2, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 3, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 4, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min,
    int v_max, const char* display_format, const char* display_format_max)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2);

    bool value_changed = DragInt("##min",
        v_current_min,
        v_speed,
        (v_min >= v_max) ? INT_MIN : v_min,
        (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max),
        display_format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragInt("##max",
        v_current_max,
        v_speed,
        (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min),
        (v_min >= v_max) ? INT_MAX : v_max,
        display_format_max ? display_format_max : display_format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();
=======
bool ImGui::DragInt(const char *label, int *v, float v_speed, int v_min,
                    int v_max, const char *display_format) {
  if (!display_format)
    display_format = "%.0f";
  float v_f = (float)*v;
  bool value_changed = DragFloat(label, &v_f, v_speed, (float)v_min,
                                 (float)v_max, display_format);
  *v = (int)v_f;
  return value_changed;
}

bool ImGui::DragIntN(const char *label, int *v, int components, float v_speed,
                     int v_min, int v_max, const char *display_format) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |=
        DragInt("##v", &v[i], v_speed, v_min, v_max, display_format);
    SameLine(0, g.Style.ItemInnerSpacing.x);
>>>>>>> master
    PopID();
    PopItemWidth();
  }
  PopID();

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

<<<<<<< HEAD
void ImGui::PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data,
    int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size                = CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f) graph_size.x = CalcItemWidth();
    if (graph_size.y == 0.0f) graph_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(
        frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, NULL)) return;

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX) {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++) {
            const float v = values_getter(data, i);
            v_min         = ImMin(v_min, v);
            v_max         = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX) scale_min = v_min;
        if (scale_max == FLT_MAX) scale_max = v_max;
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (values_count > 0) {
        int res_w      = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        // Tooltip on hover
        int v_hovered = -1;
        if (IsHovered(inner_bb, 0)) {
            const float t =
                ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
            const int v_idx = (int)(t * item_count);
            IM_ASSERT(v_idx >= 0 && v_idx < values_count);

            const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
            const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
            if (plot_type == ImGuiPlotType_Lines)
                SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx + 1, v1);
            else if (plot_type == ImGuiPlotType_Histogram)
                SetTooltip("%d: %8.4g", v_idx, v0);
            v_hovered = v_idx;
        }

        const float t_step = 1.0f / (float)res_w;

        float  v0  = values_getter(data, (0 + values_offset) % values_count);
        float  t0  = 0.0f;
        ImVec2 tp0 = ImVec2(t0, 1.0f - ImSaturate((v0 - scale_min) / (scale_max - scale_min)));  // Point in the
        // normalized space of
        // our target rectangle

        const ImU32 col_base =
            GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
        const ImU32 col_hovered =
            GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

        for (int n = 0; n < res_w; n++) {
            const float t1     = t0 + t_step;
            const int   v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float  v1  = values_getter(data, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2(t1, 1.0f - ImSaturate((v1 - scale_min) / (scale_max - scale_min)));

            // NB: Draw calls are merged together by the DrawList system. Still, we
            // should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 =
                ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, 1.0f));
            if (plot_type == ImGuiPlotType_Lines) {
                window->DrawList->AddLine(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }
            else if (plot_type == ImGuiPlotType_Histogram)
            {
                if (pos1.x >= pos0.x + 2.0f) pos1.x -= 1.0f;
                window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }

            t0  = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
            frame_bb.Max,
            overlay_text,
            NULL,
            NULL,
            ImVec2(0.5f, 0.0f));

    if (label_size.x > 0.0f) RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData
{
    const float* Values;
    int          Stride;

    ImGuiPlotArrayGetterData(const float* values, int stride)
    {
        Values = values;
        Stride = stride;
    }
};

static float Plot_ArrayGetter(void* data, int idx)
{
    ImGuiPlotArrayGetterData* plot_data = (ImGuiPlotArrayGetterData*)data;
    const float               v = *(float*)(void*)((unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void ImGui::PlotLines(const char* label, const float* values, int values_count, int values_offset,
    const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    ImGuiPlotArrayGetterData data(values, stride);
    PlotEx(ImGuiPlotType_Lines,
        label,
        &Plot_ArrayGetter,
        (void*)&data,
        values_count,
        values_offset,
        overlay_text,
        scale_min,
        scale_max,
        graph_size);
}

void ImGui::PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count,
    int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(ImGuiPlotType_Lines,
        label,
        values_getter,
        data,
        values_count,
        values_offset,
        overlay_text,
        scale_min,
        scale_max,
        graph_size);
}

void ImGui::PlotHistogram(const char* label, const float* values, int values_count, int values_offset,
    const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    ImGuiPlotArrayGetterData data(values, stride);
    PlotEx(ImGuiPlotType_Histogram,
        label,
        &Plot_ArrayGetter,
        (void*)&data,
        values_count,
        values_offset,
        overlay_text,
        scale_min,
        scale_max,
        graph_size);
}

void ImGui::PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count,
    int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(ImGuiPlotType_Histogram,
        label,
        values_getter,
        data,
        values_count,
        values_offset,
        overlay_text,
        scale_min,
        scale_max,
        graph_size);
=======
bool ImGui::DragInt2(const char *label, int v[2], float v_speed, int v_min,
                     int v_max, const char *display_format) {
  return DragIntN(label, v, 2, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt3(const char *label, int v[3], float v_speed, int v_min,
                     int v_max, const char *display_format) {
  return DragIntN(label, v, 3, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt4(const char *label, int v[4], float v_speed, int v_min,
                     int v_max, const char *display_format) {
  return DragIntN(label, v, 4, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragIntRange2(const char *label, int *v_current_min,
                          int *v_current_max, float v_speed, int v_min,
                          int v_max, const char *display_format,
                          const char *display_format_max) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  PushID(label);
  BeginGroup();
  PushMultiItemsWidths(2);

  bool value_changed = DragInt(
      "##min", v_current_min, v_speed, (v_min >= v_max) ? INT_MIN : v_min,
      (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max),
      display_format);
  PopItemWidth();
  SameLine(0, g.Style.ItemInnerSpacing.x);
  value_changed |=
      DragInt("##max", v_current_max, v_speed,
              (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min),
              (v_min >= v_max) ? INT_MAX : v_max,
              display_format_max ? display_format_max : display_format);
  PopItemWidth();
  SameLine(0, g.Style.ItemInnerSpacing.x);

  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();
  PopID();

  return value_changed;
}

void ImGui::PlotEx(ImGuiPlotType plot_type, const char *label,
                   float (*values_getter)(void *data, int idx), void *data,
                   int values_count, int values_offset,
                   const char *overlay_text, float scale_min, float scale_max,
                   ImVec2 graph_size) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  if (graph_size.x == 0.0f)
    graph_size.x = CalcItemWidth();
  if (graph_size.y == 0.0f)
    graph_size.y = label_size.y + (style.FramePadding.y * 2);

  const ImRect frame_bb(window->DC.CursorPos,
                        window->DC.CursorPos +
                            ImVec2(graph_size.x, graph_size.y));
  const ImRect inner_bb(frame_bb.Min + style.FramePadding,
                        frame_bb.Max - style.FramePadding);
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max +
                            ImVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                                   0));
  ItemSize(total_bb, style.FramePadding.y);
  if (!ItemAdd(total_bb, NULL))
    return;

  // Determine scale from values if not specified
  if (scale_min == FLT_MAX || scale_max == FLT_MAX) {
    float v_min = FLT_MAX;
    float v_max = -FLT_MAX;
    for (int i = 0; i < values_count; i++) {
      const float v = values_getter(data, i);
      v_min = ImMin(v_min, v);
      v_max = ImMax(v_max, v);
    }
    if (scale_min == FLT_MAX)
      scale_min = v_min;
    if (scale_max == FLT_MAX)
      scale_max = v_max;
  }

  RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true,
              style.FrameRounding);

  if (values_count > 0) {
    int res_w = ImMin((int)graph_size.x, values_count) +
                ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
    int item_count =
        values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

    // Tooltip on hover
    int v_hovered = -1;
    if (IsHovered(inner_bb, 0)) {
      const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) /
                                  (inner_bb.Max.x - inner_bb.Min.x),
                              0.0f, 0.9999f);
      const int v_idx = (int)(t * item_count);
      IM_ASSERT(v_idx >= 0 && v_idx < values_count);

      const float v0 =
          values_getter(data, (v_idx + values_offset) % values_count);
      const float v1 =
          values_getter(data, (v_idx + 1 + values_offset) % values_count);
      if (plot_type == ImGuiPlotType_Lines)
        SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx + 1, v1);
      else if (plot_type == ImGuiPlotType_Histogram)
        SetTooltip("%d: %8.4g", v_idx, v0);
      v_hovered = v_idx;
    }

    const float t_step = 1.0f / (float)res_w;

    float v0 = values_getter(data, (0 + values_offset) % values_count);
    float t0 = 0.0f;
    ImVec2 tp0 =
        ImVec2(t0, 1.0f - ImSaturate((v0 - scale_min) /
                                     (scale_max - scale_min))); // Point in the
    // normalized space of
    // our target rectangle

    const ImU32 col_base = GetColorU32((plot_type == ImGuiPlotType_Lines)
                                           ? ImGuiCol_PlotLines
                                           : ImGuiCol_PlotHistogram);
    const ImU32 col_hovered = GetColorU32((plot_type == ImGuiPlotType_Lines)
                                              ? ImGuiCol_PlotLinesHovered
                                              : ImGuiCol_PlotHistogramHovered);

    for (int n = 0; n < res_w; n++) {
      const float t1 = t0 + t_step;
      const int v1_idx = (int)(t0 * item_count + 0.5f);
      IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
      const float v1 =
          values_getter(data, (v1_idx + values_offset + 1) % values_count);
      const ImVec2 tp1 = ImVec2(
          t1, 1.0f - ImSaturate((v1 - scale_min) / (scale_max - scale_min)));

      // NB: Draw calls are merged together by the DrawList system. Still, we
      // should render our batch are lower level to save a bit of CPU.
      ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
      ImVec2 pos1 = ImLerp(
          inner_bb.Min, inner_bb.Max,
          (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, 1.0f));
      if (plot_type == ImGuiPlotType_Lines) {
        window->DrawList->AddLine(pos0, pos1,
                                  v_hovered == v1_idx ? col_hovered : col_base);
      } else if (plot_type == ImGuiPlotType_Histogram) {
        if (pos1.x >= pos0.x + 2.0f)
          pos1.x -= 1.0f;
        window->DrawList->AddRectFilled(
            pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
      }

      t0 = t1;
      tp0 = tp1;
    }
  }

  // Text overlay
  if (overlay_text)
    RenderTextClipped(
        ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
        frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f, 0.0f));

  if (label_size.x > 0.0f)
    RenderText(
        ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y),
        label);
}

struct ImGuiPlotArrayGetterData {
  const float *Values;
  int Stride;

  ImGuiPlotArrayGetterData(const float *values, int stride) {
    Values = values;
    Stride = stride;
  }
};

static float Plot_ArrayGetter(void *data, int idx) {
  ImGuiPlotArrayGetterData *plot_data = (ImGuiPlotArrayGetterData *)data;
  const float v = *(float *)(void *)((unsigned char *)plot_data->Values +
                                     (size_t)idx * plot_data->Stride);
  return v;
}

void ImGui::PlotLines(const char *label, const float *values, int values_count,
                      int values_offset, const char *overlay_text,
                      float scale_min, float scale_max, ImVec2 graph_size,
                      int stride) {
  ImGuiPlotArrayGetterData data(values, stride);
  PlotEx(ImGuiPlotType_Lines, label, &Plot_ArrayGetter, (void *)&data,
         values_count, values_offset, overlay_text, scale_min, scale_max,
         graph_size);
}

void ImGui::PlotLines(const char *label,
                      float (*values_getter)(void *data, int idx), void *data,
                      int values_count, int values_offset,
                      const char *overlay_text, float scale_min,
                      float scale_max, ImVec2 graph_size) {
  PlotEx(ImGuiPlotType_Lines, label, values_getter, data, values_count,
         values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotHistogram(const char *label, const float *values,
                          int values_count, int values_offset,
                          const char *overlay_text, float scale_min,
                          float scale_max, ImVec2 graph_size, int stride) {
  ImGuiPlotArrayGetterData data(values, stride);
  PlotEx(ImGuiPlotType_Histogram, label, &Plot_ArrayGetter, (void *)&data,
         values_count, values_offset, overlay_text, scale_min, scale_max,
         graph_size);
}

void ImGui::PlotHistogram(const char *label,
                          float (*values_getter)(void *data, int idx),
                          void *data, int values_count, int values_offset,
                          const char *overlay_text, float scale_min,
                          float scale_max, ImVec2 graph_size) {
  PlotEx(ImGuiPlotType_Histogram, label, values_getter, data, values_count,
         values_offset, overlay_text, scale_min, scale_max, graph_size);
>>>>>>> master
}

// size_arg (for each axis) < 0.0f: align to end, 0.0f: auto, > 0.0f: specified
// size
<<<<<<< HEAD
void ImGui::ProgressBar(float fraction, const ImVec2& size_arg, const char* overlay)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, pos + CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, NULL)) return;
=======
void ImGui::ProgressBar(float fraction, const ImVec2 &size_arg,
                        const char *overlay) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  ImVec2 pos = window->DC.CursorPos;
  ImRect bb(pos, pos + CalcItemSize(size_arg, CalcItemWidth(),
                                    g.FontSize + style.FramePadding.y * 2.0f));
  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(bb, NULL))
    return;
>>>>>>> master

  // Render
  fraction = ImSaturate(fraction);
  RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true,
              style.FrameRounding);
  bb.Reduce(ImVec2(window->BorderSize, window->BorderSize));
  const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
  RenderFrame(bb.Min, fill_br, GetColorU32(ImGuiCol_PlotHistogram), false,
              style.FrameRounding);

<<<<<<< HEAD
    // Default displaying the fraction as percentage string, but user can override
    // it
    char overlay_buf[32];
    if (!overlay) {
        ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
        overlay = overlay_buf;
=======
  // Default displaying the fraction as percentage string, but user can override
  // it
  char overlay_buf[32];
  if (!overlay) {
    ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%",
                   fraction * 100 + 0.01f);
    overlay = overlay_buf;
  }

  ImVec2 overlay_size = CalcTextSize(overlay, NULL);
  if (overlay_size.x > 0.0f)
    RenderTextClipped(
        ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x,
                       bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x),
               bb.Min.y),
        bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
}

bool ImGui::Checkbox(const char *label, bool *v) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  const ImRect check_bb(window->DC.CursorPos,
                        window->DC.CursorPos +
                            ImVec2(label_size.y + style.FramePadding.y * 2,
                                   label_size.y + style.FramePadding.y * 2));
  ItemSize(check_bb, style.FramePadding.y);

  ImRect total_bb = check_bb;
  if (label_size.x > 0)
    SameLine(0, style.ItemInnerSpacing.x);
  const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y),
                       window->DC.CursorPos + ImVec2(0, style.FramePadding.y) +
                           label_size);
  if (label_size.x > 0) {
    ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()),
             style.FramePadding.y);
    total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min),
                      ImMax(check_bb.Max, text_bb.Max));
  }

  if (!ItemAdd(total_bb, &id))
    return false;

  bool hovered, held;
  bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
  if (pressed)
    *v = !(*v);

  RenderFrame(check_bb.Min, check_bb.Max,
              GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive
                                            : hovered ? ImGuiCol_FrameBgHovered
                                                      : ImGuiCol_FrameBg),
              true, style.FrameRounding);
  if (*v) {
    const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
    const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
    window->DrawList->AddRectFilled(
        check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad),
        GetColorU32(ImGuiCol_CheckMark), style.FrameRounding);
  }

  if (g.LogEnabled)
    LogRenderedText(text_bb.GetTL(), *v ? "[x]" : "[ ]");
  if (label_size.x > 0.0f)
    RenderText(text_bb.GetTL(), label);

  return pressed;
}

bool ImGui::CheckboxFlags(const char *label, unsigned int *flags,
                          unsigned int flags_value) {
  bool v = ((*flags & flags_value) == flags_value);
  bool pressed = Checkbox(label, &v);
  if (pressed) {
    if (v)
      *flags |= flags_value;
    else
      *flags &= ~flags_value;
  }

  return pressed;
}

bool ImGui::RadioButton(const char *label, bool active) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  const ImRect check_bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(label_size.y + style.FramePadding.y * 2 - 1,
                 label_size.y + style.FramePadding.y * 2 - 1));
  ItemSize(check_bb, style.FramePadding.y);

  ImRect total_bb = check_bb;
  if (label_size.x > 0)
    SameLine(0, style.ItemInnerSpacing.x);
  const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y),
                       window->DC.CursorPos + ImVec2(0, style.FramePadding.y) +
                           label_size);
  if (label_size.x > 0) {
    ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()),
             style.FramePadding.y);
    total_bb.Add(text_bb);
  }

  if (!ItemAdd(total_bb, &id))
    return false;

  ImVec2 center = check_bb.GetCenter();
  center.x = (float)(int)center.x + 0.5f;
  center.y = (float)(int)center.y + 0.5f;
  const float radius = check_bb.GetHeight() * 0.5f;

  bool hovered, held;
  bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

  window->DrawList->AddCircleFilled(
      center, radius,
      GetColorU32((held && hovered)
                      ? ImGuiCol_FrameBgActive
                      : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg),
      16);
  if (active) {
    const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
    const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
    window->DrawList->AddCircleFilled(center, radius - pad,
                                      GetColorU32(ImGuiCol_CheckMark), 16);
  }

  if (window->Flags & ImGuiWindowFlags_ShowBorders) {
    window->DrawList->AddCircle(center + ImVec2(1, 1), radius,
                                GetColorU32(ImGuiCol_BorderShadow), 16);
    window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border),
                                16);
  }

  if (g.LogEnabled)
    LogRenderedText(text_bb.GetTL(), active ? "(x)" : "( )");
  if (label_size.x > 0.0f)
    RenderText(text_bb.GetTL(), label);

  return pressed;
}

bool ImGui::RadioButton(const char *label, int *v, int v_button) {
  const bool pressed = RadioButton(label, *v == v_button);
  if (pressed) {
    *v = v_button;
  }
  return pressed;
}

static int InputTextCalcTextLenAndLineCount(const char *text_begin,
                                            const char **out_text_end) {
  int line_count = 0;
  const char *s = text_begin;
  while (
      char c =
          *s++) // We are only matching for \n so we can ignore UTF-8 decoding
    if (c == '\n')
      line_count++;
  s--;
  if (s[0] != '\n' && s[0] != '\r')
    line_count++;
  *out_text_end = s;
  return line_count;
}

static ImVec2 InputTextCalcTextSizeW(const ImWchar *text_begin,
                                     const ImWchar *text_end,
                                     const ImWchar **remaining,
                                     ImVec2 *out_offset,
                                     bool stop_on_new_line) {
  ImFont *font = GImGui->Font;
  const float line_height = GImGui->FontSize;
  const float scale = line_height / font->FontSize;

  ImVec2 text_size = ImVec2(0, 0);
  float line_width = 0.0f;

  const ImWchar *s = text_begin;
  while (s < text_end) {
    unsigned int c = (unsigned int)(*s++);
    if (c == '\n') {
      text_size.x = ImMax(text_size.x, line_width);
      text_size.y += line_height;
      line_width = 0.0f;
      if (stop_on_new_line)
        break;
      continue;
>>>>>>> master
    }
    if (c == '\r')
      continue;

<<<<<<< HEAD
    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(
            ImVec2(ImClamp(
                       fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x),
                bb.Min.y),
            bb.Max,
            overlay,
            NULL,
            &overlay_size,
            ImVec2(0.0f, 0.5f),
            &bb);
}

bool ImGui::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g          = *GImGui;
    const ImGuiStyle& style      = g.Style;
    const ImGuiID     id         = window->GetID(label);
    const ImVec2      label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos,
        window->DC.CursorPos +
            ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0) SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y),
        window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
    if (label_size.x > 0) {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
    }

    if (!ItemAdd(total_bb, &id)) return false;

    bool hovered, held;
    bool pressed    = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) *v = !(*v);

    RenderFrame(check_bb.Min,
        check_bb.Max,
        GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg),
        true,
        style.FrameRounding);
    if (*v) {
        const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
        const float pad      = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
        window->DrawList->AddRectFilled(check_bb.Min + ImVec2(pad, pad),
            check_bb.Max - ImVec2(pad, pad),
            GetColorU32(ImGuiCol_CheckMark),
            style.FrameRounding);
    }

    if (g.LogEnabled) LogRenderedText(text_bb.GetTL(), *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f) RenderText(text_bb.GetTL(), label);

    return pressed;
}

bool ImGui::CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
    bool v       = ((*flags & flags_value) == flags_value);
    bool pressed = Checkbox(label, &v);
    if (pressed) {
        if (v)
            *flags |= flags_value;
        else
            *flags &= ~flags_value;
    }

    return pressed;
}

bool ImGui::RadioButton(const char* label, bool active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g          = *GImGui;
    const ImGuiStyle& style      = g.Style;
    const ImGuiID     id         = window->GetID(label);
    const ImVec2      label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos,
        window->DC.CursorPos +
            ImVec2(label_size.y + style.FramePadding.y * 2 - 1, label_size.y + style.FramePadding.y * 2 - 1));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0) SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y),
        window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
    if (label_size.x > 0) {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb.Add(text_bb);
    }

    if (!ItemAdd(total_bb, &id)) return false;

    ImVec2 center      = check_bb.GetCenter();
    center.x           = (float)(int)center.x + 0.5f;
    center.y           = (float)(int)center.y + 0.5f;
    const float radius = check_bb.GetHeight() * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    window->DrawList->AddCircleFilled(center,
        radius,
        GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg),
        16);
    if (active) {
        const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
        const float pad      = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
        window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(ImGuiCol_CheckMark), 16);
    }

    if (window->Flags & ImGuiWindowFlags_ShowBorders) {
        window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(ImGuiCol_BorderShadow), 16);
        window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 16);
    }

    if (g.LogEnabled) LogRenderedText(text_bb.GetTL(), active ? "(x)" : "( )");
    if (label_size.x > 0.0f) RenderText(text_bb.GetTL(), label);

    return pressed;
}

bool ImGui::RadioButton(const char* label, int* v, int v_button)
{
    const bool pressed = RadioButton(label, *v == v_button);
    if (pressed) {
        *v = v_button;
    }
    return pressed;
}

static int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
    int         line_count = 0;
    const char* s          = text_begin;
    while (char c = *s++)  // We are only matching for \n so we can ignore UTF-8 decoding
        if (c == '\n') line_count++;
    s--;
    if (s[0] != '\n' && s[0] != '\r') line_count++;
    *out_text_end = s;
    return line_count;
}

static ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining,
    ImVec2* out_offset, bool stop_on_new_line)
{
    ImFont*     font        = GImGui->Font;
    const float line_height = GImGui->FontSize;
    const float scale       = line_height / font->FontSize;

    ImVec2 text_size  = ImVec2(0, 0);
    float  line_width = 0.0f;

    const ImWchar* s = text_begin;
    while (s < text_end) {
        unsigned int c = (unsigned int)(*s++);
        if (c == '\n') {
            text_size.x = ImMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line) break;
            continue;
        }
        if (c == '\r') continue;

        const float char_width = font->GetCharAdvance((unsigned short)c) * scale;
        line_width += char_width;
    }

    if (text_size.x < line_width) text_size.x = line_width;

    if (out_offset) *out_offset = ImVec2(line_width, text_size.y + line_height);  // offset allow for the
                                                                                  // possibility of sitting
                                                                                  // after a trailing \n

    if (line_width > 0 || text_size.y == 0.0f)  // whereas size.y will ignore the trailing \n
        text_size.y += line_height;

    if (remaining) *remaining = s;

    return text_size;
}

// Wrapper for stb_textedit.h to edit text (our wrapper is for: statically sized
// buffer, single-line, wchar characters. InputText converts between UTF-8 and
// wchar)
namespace ImGuiStb
{

    static int STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING* obj) { return obj->CurLenW; }
    static ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING* obj, int idx) { return obj->Text[idx]; }
    static float STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING* obj, int line_start_idx, int char_idx)
    {
        ImWchar c = obj->Text[line_start_idx + char_idx];
        if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE;
        return GImGui->Font->GetCharAdvance(c) * (GImGui->FontSize / GImGui->Font->FontSize);
    }
    static int STB_TEXTEDIT_KEYTOTEXT(int key) { return key >= 0x10000 ? 0 : key; }
    static ImWchar                        STB_TEXTEDIT_NEWLINE = '\n';
    static void STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, STB_TEXTEDIT_STRING* obj, int line_start_idx)
    {
        const ImWchar* text           = obj->Text.Data;
        const ImWchar* text_remaining = NULL;
        const ImVec2   size =
            InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
        r->x0               = 0.0f;
        r->x1               = size.x;
        r->baseline_y_delta = size.y;
        r->ymin             = 0.0f;
        r->ymax             = size.y;
        r->num_chars        = (int)(text_remaining - (text + line_start_idx));
    }

    static bool is_separator(unsigned int c)
    {
        return ImCharIsSpace(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
               c == ']' || c == '|';
    }
    static int is_word_boundary_from_right(STB_TEXTEDIT_STRING* obj, int idx)
    {
        return idx > 0 ? (is_separator(obj->Text[idx - 1]) && !is_separator(obj->Text[idx])) : 1;
    }
    static int STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)
    {
        idx--;
        while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--;
        return idx < 0 ? 0 : idx;
    }
#ifdef __APPLE__  // FIXME: Move setting to IO structure
    static int is_word_boundary_from_left(STB_TEXTEDIT_STRING* obj, int idx)
    {
        return idx > 0 ? (!is_separator(obj->Text[idx - 1]) && is_separator(obj->Text[idx])) : 1;
    }
    static int STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)
    {
        idx++;
        int len = obj->CurLenW;
        while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++;
        return idx > len ? len : idx;
    }
#else
    static int STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)
    {
        idx++;
        int len = obj->CurLenW;
        while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++;
        return idx > len ? len : idx;
    }
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT STB_TEXTEDIT_MOVEWORDLEFT_IMPL  // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

    static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
    {
        ImWchar* dst = obj->Text.Data + pos;

        // We maintain our buffer length in both UTF-8 and wchar formats
        obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
        obj->CurLenW -= n;

        // Offset remaining text
        const ImWchar* src = obj->Text.Data + pos + n;
        while (ImWchar c = *src++) *dst++ = c;
        *dst                              = '\0';
    }

    static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
    {
        const int text_len = obj->CurLenW;
        IM_ASSERT(pos <= text_len);
        if (new_text_len + text_len + 1 > obj->Text.Size) return false;

        const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
        if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA) return false;

        ImWchar* text = obj->Text.Data;
        if (pos != text_len) memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
        memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

        obj->CurLenW += new_text_len;
        obj->CurLenA += new_text_len_utf8;
        obj->Text[obj->CurLenW] = '\0';

        return true;
    }

// We don't use an enum so we can build even with conflicting symbols (if
// another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT 0x10000       // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT 0x10001      // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP 0x10002         // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN 0x10003       // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART 0x10004  // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND 0x10005    // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART 0x10006  // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND 0x10007    // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE 0x10008     // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE 0x10009  // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO 0x1000A       // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO 0x1000B       // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT 0x1000C   // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT 0x1000D  // keyboard input to move cursor right one word
=======
    const float char_width = font->GetCharAdvance((unsigned short)c) * scale;
    line_width += char_width;
  }

  if (text_size.x < line_width)
    text_size.x = line_width;

  if (out_offset)
    *out_offset =
        ImVec2(line_width, text_size.y + line_height); // offset allow for the
                                                       // possibility of sitting
                                                       // after a trailing \n

  if (line_width > 0 ||
      text_size.y == 0.0f) // whereas size.y will ignore the trailing \n
    text_size.y += line_height;

  if (remaining)
    *remaining = s;

  return text_size;
}

// Wrapper for stb_textedit.h to edit text (our wrapper is for: statically sized
// buffer, single-line, wchar characters. InputText converts between UTF-8 and
// wchar)
namespace ImGuiStb {

static int STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING *obj) {
  return obj->CurLenW;
}
static ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING *obj, int idx) {
  return obj->Text[idx];
}
static float STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING *obj, int line_start_idx,
                                   int char_idx) {
  ImWchar c = obj->Text[line_start_idx + char_idx];
  if (c == '\n')
    return STB_TEXTEDIT_GETWIDTH_NEWLINE;
  return GImGui->Font->GetCharAdvance(c) *
         (GImGui->FontSize / GImGui->Font->FontSize);
}
static int STB_TEXTEDIT_KEYTOTEXT(int key) { return key >= 0x10000 ? 0 : key; }
static ImWchar STB_TEXTEDIT_NEWLINE = '\n';
static void STB_TEXTEDIT_LAYOUTROW(StbTexteditRow *r, STB_TEXTEDIT_STRING *obj,
                                   int line_start_idx) {
  const ImWchar *text = obj->Text.Data;
  const ImWchar *text_remaining = NULL;
  const ImVec2 size = InputTextCalcTextSizeW(
      text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
  r->x0 = 0.0f;
  r->x1 = size.x;
  r->baseline_y_delta = size.y;
  r->ymin = 0.0f;
  r->ymax = size.y;
  r->num_chars = (int)(text_remaining - (text + line_start_idx));
}

static bool is_separator(unsigned int c) {
  return ImCharIsSpace(c) || c == ',' || c == ';' || c == '(' || c == ')' ||
         c == '{' || c == '}' || c == '[' || c == ']' || c == '|';
}
static int is_word_boundary_from_right(STB_TEXTEDIT_STRING *obj, int idx) {
  return idx > 0 ? (is_separator(obj->Text[idx - 1]) &&
                    !is_separator(obj->Text[idx]))
                 : 1;
}
static int STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING *obj, int idx) {
  idx--;
  while (idx >= 0 && !is_word_boundary_from_right(obj, idx))
    idx--;
  return idx < 0 ? 0 : idx;
}
#ifdef __APPLE__ // FIXME: Move setting to IO structure
static int is_word_boundary_from_left(STB_TEXTEDIT_STRING *obj, int idx) {
  return idx > 0 ? (!is_separator(obj->Text[idx - 1]) &&
                    is_separator(obj->Text[idx]))
                 : 1;
}
static int STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING *obj, int idx) {
  idx++;
  int len = obj->CurLenW;
  while (idx < len && !is_word_boundary_from_left(obj, idx))
    idx++;
  return idx > len ? len : idx;
}
#else
static int STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING *obj, int idx) {
  idx++;
  int len = obj->CurLenW;
  while (idx < len && !is_word_boundary_from_right(obj, idx))
    idx++;
  return idx > len ? len : idx;
}
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT                                              \
  STB_TEXTEDIT_MOVEWORDLEFT_IMPL // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING *obj, int pos, int n) {
  ImWchar *dst = obj->Text.Data + pos;

  // We maintain our buffer length in both UTF-8 and wchar formats
  obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
  obj->CurLenW -= n;

  // Offset remaining text
  const ImWchar *src = obj->Text.Data + pos + n;
  while (ImWchar c = *src++)
    *dst++ = c;
  *dst = '\0';
}

static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING *obj, int pos,
                                     const ImWchar *new_text,
                                     int new_text_len) {
  const int text_len = obj->CurLenW;
  IM_ASSERT(pos <= text_len);
  if (new_text_len + text_len + 1 > obj->Text.Size)
    return false;

  const int new_text_len_utf8 =
      ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
  if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA)
    return false;

  ImWchar *text = obj->Text.Data;
  if (pos != text_len)
    memmove(text + pos + new_text_len, text + pos,
            (size_t)(text_len - pos) * sizeof(ImWchar));
  memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

  obj->CurLenW += new_text_len;
  obj->CurLenA += new_text_len_utf8;
  obj->Text[obj->CurLenW] = '\0';

  return true;
}

// We don't use an enum so we can build even with conflicting symbols (if
// another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT 0x10000  // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT 0x10001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP 0x10002    // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN 0x10003  // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART                                               \
  0x10004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND                                                 \
  0x10005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART                                               \
  0x10006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND                                                 \
  0x10007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE                                                  \
  0x10008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE                                               \
  0x10009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO 0x1000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO 0x1000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT                                                \
  0x1000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT                                               \
  0x1000D // keyboard input to move cursor right one word
>>>>>>> master
#define STB_TEXTEDIT_K_SHIFT 0x20000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"
}

void ImGuiTextEditState::OnKeyPressed(int key) {
  stb_textedit_key(this, &StbState, key);
  CursorFollow = true;
  CursorAnimReset();
}

// Public API to manipulate UTF-8 text
// We expose UTF-8 to the user (unlike the STB_TEXTEDIT_* functions which are
// manipulating wchar)
// FIXME: The existence of this rarely exercised code path is a bit of a
// nuisance.
<<<<<<< HEAD
void ImGuiTextEditCallbackData::DeleteChars(int pos, int bytes_count)
{
    IM_ASSERT(pos + bytes_count <= BufTextLen);
    char*       dst = Buf + pos;
    const char* src = Buf + pos + bytes_count;
    while (char c = *src++) *dst++ = c;
    *dst                           = '\0';

    if (CursorPos + bytes_count >= pos)
        CursorPos -= bytes_count;
    else if (CursorPos >= pos)
        CursorPos  = pos;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty                      = true;
    BufTextLen -= bytes_count;
}

void ImGuiTextEditCallbackData::InsertChars(int pos, const char* new_text, const char* new_text_end)
{
    const int new_text_len = new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
    if (new_text_len + BufTextLen + 1 >= BufSize) return;

    if (BufTextLen != pos) memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
    memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
    Buf[BufTextLen + new_text_len] = '\0';

    if (CursorPos >= pos) CursorPos += new_text_len;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty                      = true;
    BufTextLen += new_text_len;
}

// Return false to discard a character.
static bool InputTextFilterCharacter(
    unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    unsigned int c = *p_char;

    if (c < 128 && c != ' ' && !isprint((int)(c & 0xFF))) {
        bool pass = false;
        pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline));
        pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
        if (!pass) return false;
    }

    if (c >= 0xE000 && c <= 0xF8FF)  // Filter private Unicode range. I don't
                                     // imagine anybody would want to input them.
                                     // GLFW on OSX seems to send private
                                     // characters for special keys like arrow
                                     // keys.
        return false;

    if (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal |
                    ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank))
    {
        if (flags & ImGuiInputTextFlags_CharsDecimal)
            if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/'))
                return false;

        if (flags & ImGuiInputTextFlags_CharsHexadecimal)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F')) return false;

        if (flags & ImGuiInputTextFlags_CharsUppercase)
            if (c >= 'a' && c <= 'z') *p_char = (c += (unsigned int)('A' - 'a'));

        if (flags & ImGuiInputTextFlags_CharsNoBlank)
            if (ImCharIsSpace(c)) return false;
    }

    if (flags & ImGuiInputTextFlags_CallbackCharFilter) {
        ImGuiTextEditCallbackData callback_data;
        memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
        callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
        callback_data.EventChar = (ImWchar)c;
        callback_data.Flags     = flags;
        callback_data.UserData  = user_data;
        if (callback(&callback_data) != 0) return false;
        *p_char = callback_data.EventChar;
        if (!callback_data.EventChar) return false;
    }

    return true;
=======
void ImGuiTextEditCallbackData::DeleteChars(int pos, int bytes_count) {
  IM_ASSERT(pos + bytes_count <= BufTextLen);
  char *dst = Buf + pos;
  const char *src = Buf + pos + bytes_count;
  while (char c = *src++)
    *dst++ = c;
  *dst = '\0';

  if (CursorPos + bytes_count >= pos)
    CursorPos -= bytes_count;
  else if (CursorPos >= pos)
    CursorPos = pos;
  SelectionStart = SelectionEnd = CursorPos;
  BufDirty = true;
  BufTextLen -= bytes_count;
}

void ImGuiTextEditCallbackData::InsertChars(int pos, const char *new_text,
                                            const char *new_text_end) {
  const int new_text_len =
      new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
  if (new_text_len + BufTextLen + 1 >= BufSize)
    return;

  if (BufTextLen != pos)
    memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
  memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
  Buf[BufTextLen + new_text_len] = '\0';

  if (CursorPos >= pos)
    CursorPos += new_text_len;
  SelectionStart = SelectionEnd = CursorPos;
  BufDirty = true;
  BufTextLen += new_text_len;
}

// Return false to discard a character.
static bool InputTextFilterCharacter(unsigned int *p_char,
                                     ImGuiInputTextFlags flags,
                                     ImGuiTextEditCallback callback,
                                     void *user_data) {
  unsigned int c = *p_char;

  if (c < 128 && c != ' ' && !isprint((int)(c & 0xFF))) {
    bool pass = false;
    pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline));
    pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
    if (!pass)
      return false;
  }

  if (c >= 0xE000 && c <= 0xF8FF) // Filter private Unicode range. I don't
                                  // imagine anybody would want to input them.
                                  // GLFW on OSX seems to send private
                                  // characters for special keys like arrow
                                  // keys.
    return false;

  if (flags &
      (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal |
       ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank)) {
    if (flags & ImGuiInputTextFlags_CharsDecimal)
      if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') &&
          (c != '*') && (c != '/'))
        return false;

    if (flags & ImGuiInputTextFlags_CharsHexadecimal)
      if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') &&
          !(c >= 'A' && c <= 'F'))
        return false;

    if (flags & ImGuiInputTextFlags_CharsUppercase)
      if (c >= 'a' && c <= 'z')
        *p_char = (c += (unsigned int)('A' - 'a'));

    if (flags & ImGuiInputTextFlags_CharsNoBlank)
      if (ImCharIsSpace(c))
        return false;
  }

  if (flags & ImGuiInputTextFlags_CallbackCharFilter) {
    ImGuiTextEditCallbackData callback_data;
    memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
    callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
    callback_data.EventChar = (ImWchar)c;
    callback_data.Flags = flags;
    callback_data.UserData = user_data;
    if (callback(&callback_data) != 0)
      return false;
    *p_char = callback_data.EventChar;
    if (!callback_data.EventChar)
      return false;
  }

  return true;
>>>>>>> master
}

// Edit a string of text
// NB: when active, hold on a privately held copy of the text (and apply back to
// 'buf'). So changing 'buf' while active has no effect.
// FIXME: Rather messy function partly because we are doing UTF8 > uint16_t >
// UTF8 conversions on the go to more easily handle stb_textedit calls. Ideally
// we should stay in UTF-8 all the time. See
// https://github.com/nothings/stb/issues/188
<<<<<<< HEAD
bool ImGui::InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags,
    ImGuiTextEditCallback callback, void* user_data)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    IM_ASSERT(
        !((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline)));  // Can't use both
    // together (they both
    // use up/down keys)
    IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) &&
                (flags & ImGuiInputTextFlags_AllowTabInput)));  // Can't use both
                                                                // together (they
                                                                // both use tab
                                                                // key)

    ImGuiContext&     g     = *GImGui;
    const ImGuiIO&    io    = g.IO;
    const ImGuiStyle& style = g.Style;

    const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
    const bool is_editable  = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
    const bool is_password  = (flags & ImGuiInputTextFlags_Password) != 0;

    if (is_multiline)  // Open group before calling GetID() because groups tracks
                       // id created during their spawn
        BeginGroup();
    const ImGuiID id         = window->GetID(label);
    const ImVec2  label_size = CalcTextSize(label, NULL, true);
    ImVec2        size       = CalcItemSize(size_arg,
        CalcItemWidth(),
        (is_multiline ? GetTextLineHeight() * 8.0f : label_size.y) +
            style.FramePadding.y * 2.0f);  // Arbitrary default of 8 lines high for multi-line
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect total_bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

    ImGuiWindow* draw_window = window;
    if (is_multiline) {
        if (!BeginChildFrame(id, frame_bb.GetSize())) {
            EndChildFrame();
            EndGroup();
            return false;
        }
        draw_window = GetCurrentWindow();
        size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, &id)) return false;
    }

    // Password pushes a temporary font with only a fallback glyph
    if (is_password) {
        const ImFont::Glyph* glyph         = g.Font->FindGlyph('*');
        ImFont*              password_font = &g.InputTextPasswordFont;
        password_font->FontSize            = g.Font->FontSize;
        password_font->Scale               = g.Font->Scale;
        password_font->DisplayOffset       = g.Font->DisplayOffset;
        password_font->Ascent              = g.Font->Ascent;
        password_font->Descent             = g.Font->Descent;
        password_font->ContainerAtlas      = g.Font->ContainerAtlas;
        password_font->FallbackGlyph       = glyph;
        password_font->FallbackXAdvance    = glyph->XAdvance;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexXAdvance.empty() &&
                  password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // NB: we are only allowed to access 'edit_state' if we are the active widget.
    ImGuiTextEditState& edit_state = g.InputTextState;

    const bool focus_requested = FocusableItemRegister(window,
        g.ActiveId == id,
        (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) ==
            0);  // Using completion callback disable keyboard tabbing
    const bool focus_requested_by_code =
        focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
    const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

    const bool hovered = IsHovered(frame_bb, id);
    if (hovered) {
        SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }
    const bool user_clicked  = hovered && io.MouseClicked[0];
    const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id &&
                               g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");

    bool select_all = (g.ActiveId != id) && (flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
    if (focus_requested || user_clicked || user_scrolled) {
        if (g.ActiveId != id) {
            // Start edition
            // Take a copy of the initial buffer value (both in original UTF-8 format
            // and converted to wchar)
            // From the moment we focused we are ignoring the content of 'buf' (unless
            // we are in read-only mode)
            const int prev_len_w = edit_state.CurLenW;
            edit_state.Text.resize(buf_size + 1);         // wchar count <= UTF-8 count. we
                                                          // use +1 to make sure that .Data
                                                          // isn't NULL so it doesn't crash.
            edit_state.InitialText.resize(buf_size + 1);  // UTF-8. we use +1 to make
                                                          // sure that .Data isn't NULL
                                                          // so it doesn't crash.
            ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
            const char* buf_end = NULL;
            edit_state.CurLenW  = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
            edit_state.CurLenA  = (int)(buf_end - buf);  // We can't get the result from ImFormatString()
                                                         // above because it is not UTF-8 aware. Here
                                                         // we'll cut off malformed UTF-8.
            edit_state.CursorAnimReset();

            // Preserve cursor position and undo/redo stack if we come back to same
            // widget
            // FIXME: We should probably compare the whole buffer to be on the safety
            // side. Comparing buf (utf8) and edit_state.Text (wchar).
            const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
            if (recycle_state) {
                // Recycle existing cursor/selection/undo stack but clamp position
                // Note a single mouse click will override the cursor/position
                // immediately by calling stb_textedit_click handler.
                edit_state.CursorClamp();
            }
            else
            {
                edit_state.Id      = id;
                edit_state.ScrollX = 0.0f;
                stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
                if (!is_multiline && focus_requested_by_code) select_all = true;
            }
            if (flags & ImGuiInputTextFlags_AlwaysInsertMode) edit_state.StbState.insert_mode         = true;
            if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl))) select_all = true;
        }
        SetActiveID(id, window);
        FocusWindow(window);
    }
    else if (io.MouseClicked[0])
    {
        // Release focus when we click outside
        if (g.ActiveId == id) ClearActiveID();
    }

    bool value_changed = false;
    bool enter_pressed = false;

    if (g.ActiveId == id) {
        if (!is_editable && !g.ActiveIdIsJustActivated) {
            // When read-only we always use the live data passed to the function
            edit_state.Text.resize(buf_size + 1);
            const char* buf_end = NULL;
            edit_state.CurLenW  = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
            edit_state.CurLenA  = (int)(buf_end - buf);
            edit_state.CursorClamp();
        }
=======
bool ImGui::InputTextEx(const char *label, char *buf, int buf_size,
                        const ImVec2 &size_arg, ImGuiInputTextFlags flags,
                        ImGuiTextEditCallback callback, void *user_data) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) &&
              (flags & ImGuiInputTextFlags_Multiline))); // Can't use both
  // together (they both
  // use up/down keys)
  IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) &&
              (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both
                                                             // together (they
                                                             // both use tab
                                                             // key)

  ImGuiContext &g = *GImGui;
  const ImGuiIO &io = g.IO;
  const ImGuiStyle &style = g.Style;

  const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
  const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
  const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;

  if (is_multiline) // Open group before calling GetID() because groups tracks
                    // id created during their spawn
    BeginGroup();
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  ImVec2 size = CalcItemSize(
      size_arg, CalcItemWidth(),
      (is_multiline ? GetTextLineHeight() * 8.0f : label_size.y) +
          style.FramePadding.y *
              2.0f); // Arbitrary default of 8 lines high for multi-line
  const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
  const ImRect total_bb(
      frame_bb.Min,
      frame_bb.Max + ImVec2(label_size.x > 0.0f
                                ? (style.ItemInnerSpacing.x + label_size.x)
                                : 0.0f,
                            0.0f));

  ImGuiWindow *draw_window = window;
  if (is_multiline) {
    if (!BeginChildFrame(id, frame_bb.GetSize())) {
      EndChildFrame();
      EndGroup();
      return false;
    }
    draw_window = GetCurrentWindow();
    size.x -= draw_window->ScrollbarSizes.x;
  } else {
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, &id))
      return false;
  }

  // Password pushes a temporary font with only a fallback glyph
  if (is_password) {
    const ImFont::Glyph *glyph = g.Font->FindGlyph('*');
    ImFont *password_font = &g.InputTextPasswordFont;
    password_font->FontSize = g.Font->FontSize;
    password_font->Scale = g.Font->Scale;
    password_font->DisplayOffset = g.Font->DisplayOffset;
    password_font->Ascent = g.Font->Ascent;
    password_font->Descent = g.Font->Descent;
    password_font->ContainerAtlas = g.Font->ContainerAtlas;
    password_font->FallbackGlyph = glyph;
    password_font->FallbackXAdvance = glyph->XAdvance;
    IM_ASSERT(password_font->Glyphs.empty() &&
              password_font->IndexXAdvance.empty() &&
              password_font->IndexLookup.empty());
    PushFont(password_font);
  }

  // NB: we are only allowed to access 'edit_state' if we are the active widget.
  ImGuiTextEditState &edit_state = g.InputTextState;

  const bool focus_requested = FocusableItemRegister(
      window, g.ActiveId == id,
      (flags & (ImGuiInputTextFlags_CallbackCompletion |
                ImGuiInputTextFlags_AllowTabInput)) ==
          0); // Using completion callback disable keyboard tabbing
  const bool focus_requested_by_code =
      focus_requested &&
      (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
  const bool focus_requested_by_tab =
      focus_requested && !focus_requested_by_code;

  const bool hovered = IsHovered(frame_bb, id);
  if (hovered) {
    SetHoveredID(id);
    g.MouseCursor = ImGuiMouseCursor_TextInput;
  }
  const bool user_clicked = hovered && io.MouseClicked[0];
  const bool user_scrolled =
      is_multiline && g.ActiveId == 0 && edit_state.Id == id &&
      g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");

  bool select_all =
      (g.ActiveId != id) && (flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
  if (focus_requested || user_clicked || user_scrolled) {
    if (g.ActiveId != id) {
      // Start edition
      // Take a copy of the initial buffer value (both in original UTF-8 format
      // and converted to wchar)
      // From the moment we focused we are ignoring the content of 'buf' (unless
      // we are in read-only mode)
      const int prev_len_w = edit_state.CurLenW;
      edit_state.Text.resize(buf_size + 1); // wchar count <= UTF-8 count. we
                                            // use +1 to make sure that .Data
                                            // isn't NULL so it doesn't crash.
      edit_state.InitialText.resize(buf_size + 1); // UTF-8. we use +1 to make
                                                   // sure that .Data isn't NULL
                                                   // so it doesn't crash.
      ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
      const char *buf_end = NULL;
      edit_state.CurLenW = ImTextStrFromUtf8(
          edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
      edit_state.CurLenA =
          (int)(buf_end - buf); // We can't get the result from ImFormatString()
                                // above because it is not UTF-8 aware. Here
                                // we'll cut off malformed UTF-8.
      edit_state.CursorAnimReset();

      // Preserve cursor position and undo/redo stack if we come back to same
      // widget
      // FIXME: We should probably compare the whole buffer to be on the safety
      // side. Comparing buf (utf8) and edit_state.Text (wchar).
      const bool recycle_state =
          (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
      if (recycle_state) {
        // Recycle existing cursor/selection/undo stack but clamp position
        // Note a single mouse click will override the cursor/position
        // immediately by calling stb_textedit_click handler.
        edit_state.CursorClamp();
      } else {
        edit_state.Id = id;
        edit_state.ScrollX = 0.0f;
        stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
        if (!is_multiline && focus_requested_by_code)
          select_all = true;
      }
      if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
        edit_state.StbState.insert_mode = true;
      if (!is_multiline &&
          (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
        select_all = true;
    }
    SetActiveID(id, window);
    FocusWindow(window);
  } else if (io.MouseClicked[0]) {
    // Release focus when we click outside
    if (g.ActiveId == id)
      ClearActiveID();
  }
>>>>>>> master

  bool value_changed = false;
  bool enter_pressed = false;

<<<<<<< HEAD
        // Although we are active we don't prevent mouse from hovering other
        // elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected
        // vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];

        // Edit in progress
        const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
        const float mouse_y =
            (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize * 0.5f));

        const bool osx_double_click_selects_words =
            io.OSXBehaviors;  // OS X style: Double click selects by word instead of
                              // selecting whole text
        if (select_all || (hovered && !osx_double_click_selects_words && io.MouseDoubleClicked[0])) {
            edit_state.SelectAll();
            edit_state.SelectedAllMouseLock = true;
        }
        else if (hovered && osx_double_click_selects_words && io.MouseDoubleClicked[0])
        {
            // Select a word only, OS X style (by simulating keystrokes)
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
        }
        else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
        {
            stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
            edit_state.CursorAnimReset();
        }
        else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock &&
                 (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
            edit_state.CursorAnimReset();
            edit_state.CursorFollow = true;
        }
        if (edit_state.SelectedAllMouseLock && !io.MouseDown[0]) edit_state.SelectedAllMouseLock = false;

        if (io.InputCharacters[0]) {
            // Process text input (before we check for Return because using some IME
            // will effectively send a Return?)
            // We ignore CTRL inputs, but need to allow CTRL+ALT as some keyboards
            // (e.g. German) use AltGR - which is Alt+Ctrl - to input certain
            // characters.
            if (!(io.KeyCtrl && !io.KeyAlt) && is_editable) {
                for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
                    if (unsigned int c = (unsigned int)io.InputCharacters[n]) {
                        // Insert character if they pass filtering
                        if (!InputTextFilterCharacter(&c, flags, callback, user_data)) continue;
                        edit_state.OnKeyPressed((int)c);
                    }
            }

            // Consume characters
            memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
        }

        // Handle various key-presses
        bool       cancel_edit = false;
        const int  k_mask      = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_shortcut_key_only =
            (io.OSXBehaviors ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt &&
            !io.KeyShift;  // OS X style: Shortcuts using Cmd/Super instead of Ctrl
        const bool is_wordmove_key_down = io.OSXBehaviors ? io.KeyAlt : io.KeyCtrl;  // OS X style: Text editing
                                                                                     // cursor movement using Alt
                                                                                     // instead of Ctrl
        const bool is_startend_key_down = io.OSXBehaviors && io.KeySuper && !io.KeyCtrl &&
                                          !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows
                                                       // instead of Home/End

        if (IsKeyPressedMap(ImGuiKey_LeftArrow)) {
            edit_state.OnKeyPressed(
                (is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT
                                                                                        : STB_TEXTEDIT_K_LEFT) |
                k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_RightArrow))
        {
            edit_state.OnKeyPressed(
                (is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT
                                                                                      : STB_TEXTEDIT_K_RIGHT) |
                k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline)
        {
            if (io.KeyCtrl)
                SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f));
            else
                edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline)
        {
            if (io.KeyCtrl)
                SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY()));
            else
                edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_Home))
        {
            edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_End))
        {
            edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable)
        {
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
        {
            if (!edit_state.HasSelection()) {
                if (is_wordmove_key_down)
                    edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
                else if (io.OSXBehaviors && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
                    edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
            }
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_Enter))
        {
            bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl)) {
                ClearActiveID();
                enter_pressed = true;
            }
            else if (is_editable)
            {
                unsigned int c = '\n';  // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, user_data)) edit_state.OnKeyPressed((int)c);
            }
        }
        else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl &&
                 !io.KeyShift && !io.KeyAlt && is_editable)
        {
            unsigned int c = '\t';  // Insert TAB
            if (InputTextFilterCharacter(&c, flags, callback, user_data)) edit_state.OnKeyPressed((int)c);
        }
        else if (IsKeyPressedMap(ImGuiKey_Escape))
        {
            ClearActiveID();
            cancel_edit = true;
        }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Z) && is_editable)
        {
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO);
            edit_state.ClearSelection();
        }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Y) && is_editable)
        {
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO);
            edit_state.ClearSelection();
        }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_A))
        {
            edit_state.SelectAll();
            edit_state.CursorFollow = true;
        }
        else if (is_shortcut_key_only && !is_password &&
                 ((IsKeyPressedMap(ImGuiKey_X) && is_editable) || IsKeyPressedMap(ImGuiKey_C)) &&
                 (!is_multiline || edit_state.HasSelection()))
        {
            // Cut, Copy
            const bool cut = IsKeyPressedMap(ImGuiKey_X);
            if (cut && !edit_state.HasSelection()) edit_state.SelectAll();

            if (io.SetClipboardTextFn) {
                const int ib = edit_state.HasSelection()
                                   ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end)
                                   : 0;
                const int ie = edit_state.HasSelection()
                                   ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end)
                                   : edit_state.CurLenW;
                edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
                ImTextStrToUtf8(edit_state.TempTextBuffer.Data,
                    edit_state.TempTextBuffer.Size,
                    edit_state.Text.Data + ib,
                    edit_state.Text.Data + ie);
                SetClipboardText(edit_state.TempTextBuffer.Data);
            }

            if (cut) {
                edit_state.CursorFollow = true;
                stb_textedit_cut(&edit_state, &edit_state.StbState);
            }
        }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_V) && is_editable)
        {
            // Paste
            if (const char* clipboard = GetClipboardText()) {
                // Filter pasted buffer
                const int clipboard_len          = (int)strlen(clipboard);
                ImWchar*  clipboard_filtered     = (ImWchar*)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
                int       clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s;) {
                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0) break;
                    if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data)) continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0)  // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
                    edit_state.CursorFollow = true;
                }
                ImGui::MemFree(clipboard_filtered);
            }
        }

        if (cancel_edit) {
            // Restore initial value
            if (is_editable) {
                ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
                value_changed = true;
            }
        }
        else
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets
            // priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText,
            // making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should
            // mark dirty state from the stb_textedit callbacks.
            if (is_editable) {
                edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
                ImTextStrToUtf8(
                    edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
            }

            // User callback
            if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory |
                             ImGuiInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that
                // Completion needs to disable keyboard TABBING at the moment.
                ImGuiInputTextFlags event_flag = 0;
                ImGuiKey            event_key  = ImGuiKey_COUNT;
                if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab)) {
                    event_flag = ImGuiInputTextFlags_CallbackCompletion;
                    event_key  = ImGuiKey_Tab;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key  = ImGuiKey_UpArrow;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key  = ImGuiKey_DownArrow;
                }
                else if (flags & ImGuiInputTextFlags_CallbackAlways)
                    event_flag = ImGuiInputTextFlags_CallbackAlways;

                if (event_flag) {
                    ImGuiTextEditCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags     = flags;
                    callback_data.UserData  = user_data;
                    callback_data.ReadOnly  = !is_editable;

                    callback_data.EventKey   = event_key;
                    callback_data.Buf        = edit_state.TempTextBuffer.Data;
                    callback_data.BufTextLen = edit_state.CurLenA;
                    callback_data.BufSize    = edit_state.BufSizeA;
                    callback_data.BufDirty   = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which
                    // can be pretty slow (an incentive to ditch the ImWchar buffer, see
                    // https://github.com/nothings/stb/issues/188)
                    ImWchar*  text            = edit_state.Text.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos =
                        ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart =
                        ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd =
                        ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data);  // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
                    IM_ASSERT(callback_data.Flags == flags);
                    if (callback_data.CursorPos != utf8_cursor_pos)
                        edit_state.StbState.cursor =
                            ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
                    if (callback_data.SelectionStart != utf8_selection_start)
                        edit_state.StbState.select_start = ImTextCountCharsFromUtf8(
                            callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
                    if (callback_data.SelectionEnd != utf8_selection_end)
                        edit_state.StbState.select_end =
                            ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
                    if (callback_data.BufDirty) {
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf));  // You need to maintain
                                                                                                // BufTextLen if you
                                                                                                // change the text!
                        edit_state.CurLenW =
                            ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
                        edit_state.CurLenA = callback_data.BufTextLen;  // Assume correct length and valid
                                                                        // UTF-8 from user, saves us an extra
                                                                        // strlen()
                        edit_state.CursorAnimReset();
                    }
                }
            }

            // Copy back to user buffer
            if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0) {
                ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
                value_changed = true;
            }
        }
    }

    // Render
    // Select which buffer we are going to display. When
    // ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value.
    // We set buf to NULL to prevent accidental usage from now on.
    const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf;
    buf                     = NULL;
=======
  if (g.ActiveId == id) {
    if (!is_editable && !g.ActiveIdIsJustActivated) {
      // When read-only we always use the live data passed to the function
      edit_state.Text.resize(buf_size + 1);
      const char *buf_end = NULL;
      edit_state.CurLenW = ImTextStrFromUtf8(
          edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
      edit_state.CurLenA = (int)(buf_end - buf);
      edit_state.CursorClamp();
    }

    edit_state.BufSizeA = buf_size;
>>>>>>> master

    // Although we are active we don't prevent mouse from hovering other
    // elements unless we are interacting right now with the widget.
    // Down the line we should have a cleaner library-wide concept of Selected
    // vs Active.
    g.ActiveIdAllowOverlap = !io.MouseDown[0];

<<<<<<< HEAD
    const ImVec4 clip_rect(frame_bb.Min.x,
        frame_bb.Min.y,
        frame_bb.Min.x + size.x,
        frame_bb.Min.y + size.y);  // Not using frame_bb.Max because we have adjusted size
    ImVec2     render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2     text_size(0.f, 0.f);
    const bool is_currently_scrolling =
        (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
    if (g.ActiveId == id || is_currently_scrolling) {
        edit_state.CursorAnim += io.DeltaTime;

        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all
        // requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the
        // computation cost (non-negligible for large amount of text) + 2nd pass for
        // selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain
        // cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = edit_state.Text.Data;
        ImVec2         cursor_offset, select_start_offset;

        {
            // Count lines + find lines numbers straddling 'cursor' and 'select_start'
            // position.
            const ImWchar* searches_input_ptr[2];
            searches_input_ptr[0]              = text_begin + edit_state.StbState.cursor;
            searches_input_ptr[1]              = NULL;
            int searches_remaining             = 1;
            int searches_result_line_number[2] = {-1, -999};
            if (edit_state.StbState.select_start != edit_state.StbState.select_end) {
                searches_input_ptr[1] =
                    text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
                searches_result_line_number[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted,
            // so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n') {
                    line_count++;
                    if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0]) {
                        searches_result_line_number[0] = line_count;
                        if (--searches_remaining <= 0) break;
                    }
                    if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1]) {
                        searches_result_line_number[1] = line_count;
                        if (--searches_remaining <= 0) break;
                    }
                }
            line_count++;
            if (searches_result_line_number[0] == -1) searches_result_line_number[0] = line_count;
            if (searches_result_line_number[1] == -1) searches_result_line_number[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and
            // measuring distance
            cursor_offset.x =
                InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
            cursor_offset.y = searches_result_line_number[0] * g.FontSize;
            if (searches_result_line_number[1] >= 0) {
                select_start_offset.x =
                    InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
                select_start_offset.y = searches_result_line_number[1] * g.FontSize;
            }

            // Calculate text height
            if (is_multiline) text_size = ImVec2(size.x, line_count * g.FontSize);
        }

        // Scroll
        if (edit_state.CursorFollow) {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll)) {
                const float scroll_increment_x = size.x * 0.25f;
                if (cursor_offset.x < edit_state.ScrollX)
                    edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
                else if (cursor_offset.x - size.x >= edit_state.ScrollX)
                    edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
            }
            else
            {
                edit_state.ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline) {
                float scroll_y = draw_window->Scroll.y;
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - size.y >= scroll_y)
                    scroll_y = cursor_offset.y - size.y;
                draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y);  // To avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
                render_pos.y          = draw_window->DC.CursorPos.y;
            }
        }
        edit_state.CursorFollow    = false;
        const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

        // Draw selection
        if (edit_state.StbState.select_start != edit_state.StbState.select_end) {
            const ImWchar* text_selected_begin =
                text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
            const ImWchar* text_selected_end =
                text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

            float bg_offy_up = is_multiline ? 0.0f : -1.0f;  // FIXME: those offsets should be part of
                                                             // the style? they don't play so well
                                                             // with multi-line selection.
            float  bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImU32  bg_color   = GetColorU32(ImGuiCol_TextSelectedBg);
            ImVec2 rect_pos   = render_pos + select_start_offset - render_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end;) {
                if (rect_pos.y > clip_rect.w + g.FontSize) break;
                if (rect_pos.y < clip_rect.y) {
                    while (p < text_selected_end)
                        if (*p++ == '\n') break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f)
                        rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') *
                                                   0.50f);  // So we can see selected empty lines
                    ImRect rect(
                        rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
                    rect.Clip(clip_rect);
                    if (rect.Overlaps(clip_rect)) draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = render_pos.x - render_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        draw_window->DrawList->AddText(g.Font,
            g.FontSize,
            render_pos - render_scroll,
            GetColorU32(ImGuiCol_Text),
            buf_display,
            buf_display + edit_state.CurLenA,
            0.0f,
            is_multiline ? NULL : &clip_rect);

        // Draw blinking cursor
        bool cursor_is_visible =
            (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
        ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
        ImRect cursor_screen_rect(cursor_screen_pos.x,
            cursor_screen_pos.y - g.FontSize + 0.5f,
            cursor_screen_pos.x + 1.0f,
            cursor_screen_pos.y - 1.5f);
        if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
            draw_window->DrawList->AddLine(
                cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));

        // Notify OS of text input position for advanced IME (-1 x offset so that
        // Windows IME can cover our cursor. Bit of an extra nicety.)
        if (is_editable) g.OsImePosRequest = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
    }
    else
    {
        // Render text only
        const char* buf_end = NULL;
        if (is_multiline)
            text_size = ImVec2(
                size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize);  // We don't need width
        draw_window->DrawList->AddText(g.Font,
            g.FontSize,
            render_pos,
            GetColorU32(ImGuiCol_Text),
            buf_display,
            buf_end,
            0.0f,
            is_multiline ? NULL : &clip_rect);
    }

    if (is_multiline) {
        Dummy(text_size + ImVec2(0.0f, g.FontSize));  // Always add room to scroll an extra line
        EndChildFrame();
        EndGroup();
    }

    if (is_password) PopFont();

    // Log as text
    if (g.LogEnabled && !is_password) LogRenderedText(render_pos, buf_display, NULL);

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
        return enter_pressed;
    else
        return value_changed;
}

bool ImGui::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags,
    ImGuiTextEditCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline));  // call InputTextMultiline()
    return InputTextEx(label, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
}

bool ImGui::InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size,
    ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    return InputTextEx(label, buf, (int)buf_size, size, flags | ImGuiInputTextFlags_Multiline, callback, user_data);
}

// NB: scalar_format here must be a simple "%xx" format string with no
// prefix/suffix (unlike the Drag/Slider functions "display_format" argument)
bool ImGui::InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr,
    void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g          = *GImGui;
    const ImGuiStyle& style      = g.Style;
    const ImVec2      label_size = CalcTextSize(label, NULL, true);

    BeginGroup();
    PushID(label);
    const ImVec2 button_sz = ImVec2(g.FontSize, g.FontSize) + style.FramePadding * 2.0f;
    if (step_ptr) PushItemWidth(ImMax(1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x) * 2));

    char buf[64];
    DataTypeFormatString(data_type, data_ptr, scalar_format, buf, IM_ARRAYSIZE(buf));

    bool value_changed = false;
    if (!(extra_flags & ImGuiInputTextFlags_CharsHexadecimal)) extra_flags |= ImGuiInputTextFlags_CharsDecimal;
    extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
    if (InputText("", buf, IM_ARRAYSIZE(buf), extra_flags))  // PushId(label) + ""
                                                             // gives us the
                                                             // expected ID from
                                                             // outside point of
                                                             // view
        value_changed = DataTypeApplyOpFromText(
            buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, scalar_format);

    // Step buttons
    if (step_ptr) {
        PopItemWidth();
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("-", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups)) {
            DataTypeApplyOp(data_type, '-', data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
            value_changed = true;
        }
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("+", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups)) {
            DataTypeApplyOp(data_type, '+', data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
            value_changed = true;
        }
    }
    PopID();

    if (label_size.x > 0) {
        SameLine(0, style.ItemInnerSpacing.x);
        RenderText(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), label);
        ItemSize(label_size, style.FramePadding.y);
    }
    EndGroup();

    return value_changed;
}

bool ImGui::InputFloat(
    const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    char display_format[16];
    if (decimal_precision < 0) strcpy(display_format, "%f");  // Ideally we'd have a minimum decimal
                                                              // precision of 1 to visually denote that this
                                                              // is a float, while hiding non-significant
                                                              // digits? %f doesn't have a minimum of 1
    else
        ImFormatString(display_format, IM_ARRAYSIZE(display_format), "%%.%df", decimal_precision);
    return InputScalarEx(label,
        ImGuiDataType_Float,
        (void*)v,
        (void*)(step > 0.0f ? &step : NULL),
        (void*)(step_fast > 0.0f ? &step_fast : NULL),
        display_format,
        extra_flags);
}

bool ImGui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags extra_flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward.
    // Typically you'd use InputText() to parse your own data, if you want to
    // handle prefixes.
    const char* scalar_format = (extra_flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return InputScalarEx(label,
        ImGuiDataType_Int,
        (void*)v,
        (void*)(step > 0.0f ? &step : NULL),
        (void*)(step_fast > 0.0f ? &step_fast : NULL),
        scalar_format,
        extra_flags);
}

bool ImGui::InputFloatN(
    const char* label, float* v, int components, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= InputFloat("##v", &v[i], 0, 0, decimal_precision, extra_flags);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 2, decimal_precision, extra_flags);
}

bool ImGui::InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 3, decimal_precision, extra_flags);
}

bool ImGui::InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 4, decimal_precision, extra_flags);
}

bool ImGui::InputIntN(const char* label, int* v, int components, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    bool          value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++) {
        PushID(i);
        value_changed |= InputInt("##v", &v[i], 0, 0, extra_flags);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 2, extra_flags);
}

bool ImGui::InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 3, extra_flags);
}

bool ImGui::InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 4, extra_flags);
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text) *out_text  = items[idx];
    return true;
}

static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1
    // active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int         items_count              = 0;
    const char* p                        = items_separated_by_zeros;
    while (*p) {
        if (idx == items_count) break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p) return false;
    if (out_text) *out_text = p;
    return true;
}

// Combo box helper allowing to pass an array of strings.
bool ImGui::Combo(const char* label, int* current_item, const char* const* items, int items_count, int height_in_items)
{
    const bool value_changed =
        Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string.
bool ImGui::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int         items_count = 0;
    const char* p           = items_separated_by_zeros;  // FIXME-OPT: Avoid computing this,
                                                         // or at least only when combo is
                                                         // open
    while (*p) {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(
        label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

// Combo box function.
bool ImGui::Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data,
    int items_count, int height_in_items)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);
    const float       w     = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(
        window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, &id)) return false;

    const float arrow_size       = (g.FontSize + style.FramePadding.x * 2.0f);
    const bool  hovered          = IsHovered(frame_bb, id);
    bool        popup_open       = IsPopupOpen(id);
    bool        popup_opened_now = false;

    const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    RenderFrame(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y),
        frame_bb.Max,
        GetColorU32(popup_open || hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button),
        true,
        style.FrameRounding);  // FIXME-ROUNDING
    RenderCollapseTriangle(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y) + style.FramePadding, true);

    if (*current_item >= 0 && *current_item < items_count) {
        const char* item_text;
        if (items_getter(data, *current_item, &item_text))
            RenderTextClipped(
                frame_bb.Min + style.FramePadding, value_bb.Max, item_text, NULL, NULL, ImVec2(0.0f, 0.0f));
    }

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if (hovered) {
        SetHoveredID(id);
        if (g.IO.MouseClicked[0]) {
            ClearActiveID();
            if (IsPopupOpen(id)) {
                ClosePopup(id);
            }
            else
            {
                FocusWindow(window);
                OpenPopup(label);
                popup_open = popup_opened_now = true;
            }
        }
    }

    bool value_changed = false;
    if (IsPopupOpen(id)) {
        // Size default to hold ~7 items
        if (height_in_items < 0) height_in_items = 7;

        float popup_height =
            (label_size.y + style.ItemSpacing.y) * ImMin(items_count, height_in_items) + (style.FramePadding.y * 3);
        float popup_y1 = frame_bb.Max.y;
        float popup_y2 =
            ImClamp(popup_y1 + popup_height, popup_y1, g.IO.DisplaySize.y - style.DisplaySafeAreaPadding.y);
        if ((popup_y2 - popup_y1) < ImMin(popup_height, frame_bb.Min.y - style.DisplaySafeAreaPadding.y)) {
            // Position our combo ABOVE because there's more space to fit! (FIXME:
            // Handle in Begin() or use a shared helper. We have similar code in
            // Begin() for popup placement)
            popup_y1 = ImClamp(frame_bb.Min.y - popup_height, style.DisplaySafeAreaPadding.y, frame_bb.Min.y);
            popup_y2 = frame_bb.Min.y;
        }
        ImRect popup_rect(ImVec2(frame_bb.Min.x, popup_y1), ImVec2(frame_bb.Max.x, popup_y2));
        SetNextWindowPos(popup_rect.Min);
        SetNextWindowSize(popup_rect.GetSize());
        PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);

        const ImGuiWindowFlags flags =
            ImGuiWindowFlags_ComboBox |
            ((window->Flags & ImGuiWindowFlags_ShowBorders) ? ImGuiWindowFlags_ShowBorders : 0);
        if (BeginPopupEx(label, flags)) {
            // Display items
            Spacing();
            for (int i = 0; i < items_count; i++) {
                PushID((void*)(intptr_t)i);
                const bool  item_selected = (i == *current_item);
                const char* item_text;
                if (!items_getter(data, i, &item_text)) item_text = "*Unknown item*";
                if (Selectable(item_text, item_selected)) {
                    ClearActiveID();
                    value_changed = true;
                    *current_item = i;
                }
                if (item_selected && popup_opened_now) SetScrollHere();
                PopID();
            }
            EndPopup();
        }
        PopStyleVar();
    }
    return value_changed;
}

// Tip: pass an empty label (e.g. "##dummy") then you can use the space to draw
// other text or image.
// But you need to make sure the ID is unique, e.g. enclose calls in
// PushID/PopID.
bool ImGui::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1) PopClipRect();

    ImGuiID id         = window->GetID(label);
    ImVec2  label_size = CalcTextSize(label, NULL, true);
    ImVec2  size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2  pos = window->DC.CursorPos;
    pos.y += window->DC.CurrentLineTextBaseOffset;
    ImRect bb(pos, pos + size);
    ItemSize(bb);

    // Fill horizontal space.
    ImVec2 window_padding = window->WindowPadding;
    float  max_x =
        (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
    float  w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
    ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw,
        size_arg.y != 0.0f ? size_arg.y : size.y);
    ImRect bb_with_spacing(pos, pos + size_draw);
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
        bb_with_spacing.Max.x += window_padding.x;

    // Selectables are tightly packed together, we extend the box to cover spacing
    // between selectable.
    float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
    float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
    float spacing_R = style.ItemSpacing.x - spacing_L;
    float spacing_D = style.ItemSpacing.y - spacing_U;
    bb_with_spacing.Min.x -= spacing_L;
    bb_with_spacing.Min.y -= spacing_U;
    bb_with_spacing.Max.x += spacing_R;
    bb_with_spacing.Max.y += spacing_D;
    if (!ItemAdd(bb_with_spacing, &id)) {
        if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1) PushColumnClipRect();
        return false;
    }

    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick;
    if (flags & ImGuiSelectableFlags_MenuItem)
        button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease;
    if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
    if (flags & ImGuiSelectableFlags_AllowDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
    if (flags & ImGuiSelectableFlags_Disabled) selected = false;

    // Render
    if (hovered || selected) {
        const ImU32 col =
            GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
    }

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1) {
        PushColumnClipRect();
        bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
    }

    if (flags & ImGuiSelectableFlags_Disabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size, ImVec2(0.0f, 0.0f));
    if (flags & ImGuiSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && !(flags & ImGuiSelectableFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
        CloseCurrentPopup();
    return pressed;
}

bool ImGui::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (Selectable(label, *p_selected, flags, size_arg)) {
        *p_selected = !*p_selected;
        return true;
    }
=======
    // Edit in progress
    const float mouse_x =
        (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) +
        edit_state.ScrollX;
    const float mouse_y =
        (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y -
                         style.FramePadding.y)
                      : (g.FontSize * 0.5f));

    const bool osx_double_click_selects_words =
        io.OSXBehaviors; // OS X style: Double click selects by word instead of
                         // selecting whole text
    if (select_all || (hovered && !osx_double_click_selects_words &&
                       io.MouseDoubleClicked[0])) {
      edit_state.SelectAll();
      edit_state.SelectedAllMouseLock = true;
    } else if (hovered && osx_double_click_selects_words &&
               io.MouseDoubleClicked[0]) {
      // Select a word only, OS X style (by simulating keystrokes)
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
    } else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock) {
      stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
      edit_state.CursorAnimReset();
    } else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock &&
               (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)) {
      stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
      edit_state.CursorAnimReset();
      edit_state.CursorFollow = true;
    }
    if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
      edit_state.SelectedAllMouseLock = false;

    if (io.InputCharacters[0]) {
      // Process text input (before we check for Return because using some IME
      // will effectively send a Return?)
      // We ignore CTRL inputs, but need to allow CTRL+ALT as some keyboards
      // (e.g. German) use AltGR - which is Alt+Ctrl - to input certain
      // characters.
      if (!(io.KeyCtrl && !io.KeyAlt) && is_editable) {
        for (int n = 0;
             n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
          if (unsigned int c = (unsigned int)io.InputCharacters[n]) {
            // Insert character if they pass filtering
            if (!InputTextFilterCharacter(&c, flags, callback, user_data))
              continue;
            edit_state.OnKeyPressed((int)c);
          }
      }

      // Consume characters
      memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
    }

    // Handle various key-presses
    bool cancel_edit = false;
    const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
    const bool is_shortcut_key_only =
        (io.OSXBehaviors ? (io.KeySuper && !io.KeyCtrl)
                         : (io.KeyCtrl && !io.KeySuper)) &&
        !io.KeyAlt &&
        !io.KeyShift; // OS X style: Shortcuts using Cmd/Super instead of Ctrl
    const bool is_wordmove_key_down =
        io.OSXBehaviors ? io.KeyAlt : io.KeyCtrl; // OS X style: Text editing
                                                  // cursor movement using Alt
                                                  // instead of Ctrl
    const bool is_startend_key_down =
        io.OSXBehaviors && io.KeySuper && !io.KeyCtrl &&
        !io.KeyAlt; // OS X style: Line/Text Start and End using Cmd+Arrows
                    // instead of Home/End

    if (IsKeyPressedMap(ImGuiKey_LeftArrow)) {
      edit_state.OnKeyPressed(
          (is_startend_key_down ? STB_TEXTEDIT_K_LINESTART
                                : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT
                                                       : STB_TEXTEDIT_K_LEFT) |
          k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_RightArrow)) {
      edit_state.OnKeyPressed((is_startend_key_down
                                   ? STB_TEXTEDIT_K_LINEEND
                                   : is_wordmove_key_down
                                         ? STB_TEXTEDIT_K_WORDRIGHT
                                         : STB_TEXTEDIT_K_RIGHT) |
                              k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline) {
      if (io.KeyCtrl)
        SetWindowScrollY(draw_window,
                         ImMax(draw_window->Scroll.y - g.FontSize, 0.0f));
      else
        edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART
                                                      : STB_TEXTEDIT_K_UP) |
                                k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline) {
      if (io.KeyCtrl)
        SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize,
                                            GetScrollMaxY()));
      else
        edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND
                                                      : STB_TEXTEDIT_K_DOWN) |
                                k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_Home)) {
      edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask
                                         : STB_TEXTEDIT_K_LINESTART | k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_End)) {
      edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask
                                         : STB_TEXTEDIT_K_LINEEND | k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable) {
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable) {
      if (!edit_state.HasSelection()) {
        if (is_wordmove_key_down)
          edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT |
                                  STB_TEXTEDIT_K_SHIFT);
        else if (io.OSXBehaviors && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
          edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART |
                                  STB_TEXTEDIT_K_SHIFT);
      }
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
    } else if (IsKeyPressedMap(ImGuiKey_Enter)) {
      bool ctrl_enter_for_new_line =
          (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
      if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) ||
          (!ctrl_enter_for_new_line && io.KeyCtrl)) {
        ClearActiveID();
        enter_pressed = true;
      } else if (is_editable) {
        unsigned int c = '\n'; // Insert new line
        if (InputTextFilterCharacter(&c, flags, callback, user_data))
          edit_state.OnKeyPressed((int)c);
      }
    } else if ((flags & ImGuiInputTextFlags_AllowTabInput) &&
               IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift &&
               !io.KeyAlt && is_editable) {
      unsigned int c = '\t'; // Insert TAB
      if (InputTextFilterCharacter(&c, flags, callback, user_data))
        edit_state.OnKeyPressed((int)c);
    } else if (IsKeyPressedMap(ImGuiKey_Escape)) {
      ClearActiveID();
      cancel_edit = true;
    } else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Z) &&
               is_editable) {
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO);
      edit_state.ClearSelection();
    } else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Y) &&
               is_editable) {
      edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO);
      edit_state.ClearSelection();
    } else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_A)) {
      edit_state.SelectAll();
      edit_state.CursorFollow = true;
    } else if (is_shortcut_key_only && !is_password &&
               ((IsKeyPressedMap(ImGuiKey_X) && is_editable) ||
                IsKeyPressedMap(ImGuiKey_C)) &&
               (!is_multiline || edit_state.HasSelection())) {
      // Cut, Copy
      const bool cut = IsKeyPressedMap(ImGuiKey_X);
      if (cut && !edit_state.HasSelection())
        edit_state.SelectAll();

      if (io.SetClipboardTextFn) {
        const int ib = edit_state.HasSelection()
                           ? ImMin(edit_state.StbState.select_start,
                                   edit_state.StbState.select_end)
                           : 0;
        const int ie = edit_state.HasSelection()
                           ? ImMax(edit_state.StbState.select_start,
                                   edit_state.StbState.select_end)
                           : edit_state.CurLenW;
        edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
        ImTextStrToUtf8(edit_state.TempTextBuffer.Data,
                        edit_state.TempTextBuffer.Size,
                        edit_state.Text.Data + ib, edit_state.Text.Data + ie);
        SetClipboardText(edit_state.TempTextBuffer.Data);
      }

      if (cut) {
        edit_state.CursorFollow = true;
        stb_textedit_cut(&edit_state, &edit_state.StbState);
      }
    } else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_V) &&
               is_editable) {
      // Paste
      if (const char *clipboard = GetClipboardText()) {
        // Filter pasted buffer
        const int clipboard_len = (int)strlen(clipboard);
        ImWchar *clipboard_filtered =
            (ImWchar *)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
        int clipboard_filtered_len = 0;
        for (const char *s = clipboard; *s;) {
          unsigned int c;
          s += ImTextCharFromUtf8(&c, s, NULL);
          if (c == 0)
            break;
          if (c >= 0x10000 ||
              !InputTextFilterCharacter(&c, flags, callback, user_data))
            continue;
          clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
        }
        clipboard_filtered[clipboard_filtered_len] = 0;
        if (clipboard_filtered_len >
            0) // If everything was filtered, ignore the pasting operation
        {
          stb_textedit_paste(&edit_state, &edit_state.StbState,
                             clipboard_filtered, clipboard_filtered_len);
          edit_state.CursorFollow = true;
        }
        ImGui::MemFree(clipboard_filtered);
      }
    }

    if (cancel_edit) {
      // Restore initial value
      if (is_editable) {
        ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
        value_changed = true;
      }
    } else {
      // Apply new value immediately - copy modified buffer back
      // Note that as soon as the input box is active, the in-widget value gets
      // priority over any underlying modification of the input buffer
      // FIXME: We actually always render 'buf' when calling DrawList->AddText,
      // making the comment above incorrect.
      // FIXME-OPT: CPU waste to do this every time the widget is active, should
      // mark dirty state from the stb_textedit callbacks.
      if (is_editable) {
        edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
        ImTextStrToUtf8(edit_state.TempTextBuffer.Data,
                        edit_state.TempTextBuffer.Size, edit_state.Text.Data,
                        NULL);
      }

      // User callback
      if ((flags & (ImGuiInputTextFlags_CallbackCompletion |
                    ImGuiInputTextFlags_CallbackHistory |
                    ImGuiInputTextFlags_CallbackAlways)) != 0) {
        IM_ASSERT(callback != NULL);

        // The reason we specify the usage semantic (Completion/History) is that
        // Completion needs to disable keyboard TABBING at the moment.
        ImGuiInputTextFlags event_flag = 0;
        ImGuiKey event_key = ImGuiKey_COUNT;
        if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 &&
            IsKeyPressedMap(ImGuiKey_Tab)) {
          event_flag = ImGuiInputTextFlags_CallbackCompletion;
          event_key = ImGuiKey_Tab;
        } else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 &&
                   IsKeyPressedMap(ImGuiKey_UpArrow)) {
          event_flag = ImGuiInputTextFlags_CallbackHistory;
          event_key = ImGuiKey_UpArrow;
        } else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 &&
                   IsKeyPressedMap(ImGuiKey_DownArrow)) {
          event_flag = ImGuiInputTextFlags_CallbackHistory;
          event_key = ImGuiKey_DownArrow;
        } else if (flags & ImGuiInputTextFlags_CallbackAlways)
          event_flag = ImGuiInputTextFlags_CallbackAlways;

        if (event_flag) {
          ImGuiTextEditCallbackData callback_data;
          memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
          callback_data.EventFlag = event_flag;
          callback_data.Flags = flags;
          callback_data.UserData = user_data;
          callback_data.ReadOnly = !is_editable;

          callback_data.EventKey = event_key;
          callback_data.Buf = edit_state.TempTextBuffer.Data;
          callback_data.BufTextLen = edit_state.CurLenA;
          callback_data.BufSize = edit_state.BufSizeA;
          callback_data.BufDirty = false;

          // We have to convert from wchar-positions to UTF-8-positions, which
          // can be pretty slow (an incentive to ditch the ImWchar buffer, see
          // https://github.com/nothings/stb/issues/188)
          ImWchar *text = edit_state.Text.Data;
          const int utf8_cursor_pos = callback_data.CursorPos =
              ImTextCountUtf8BytesFromStr(text,
                                          text + edit_state.StbState.cursor);
          const int utf8_selection_start = callback_data.SelectionStart =
              ImTextCountUtf8BytesFromStr(
                  text, text + edit_state.StbState.select_start);
          const int utf8_selection_end = callback_data.SelectionEnd =
              ImTextCountUtf8BytesFromStr(
                  text, text + edit_state.StbState.select_end);

          // Call user code
          callback(&callback_data);

          // Read back what user may have modified
          IM_ASSERT(
              callback_data.Buf ==
              edit_state.TempTextBuffer.Data); // Invalid to modify those fields
          IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
          IM_ASSERT(callback_data.Flags == flags);
          if (callback_data.CursorPos != utf8_cursor_pos)
            edit_state.StbState.cursor = ImTextCountCharsFromUtf8(
                callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
          if (callback_data.SelectionStart != utf8_selection_start)
            edit_state.StbState.select_start = ImTextCountCharsFromUtf8(
                callback_data.Buf,
                callback_data.Buf + callback_data.SelectionStart);
          if (callback_data.SelectionEnd != utf8_selection_end)
            edit_state.StbState.select_end = ImTextCountCharsFromUtf8(
                callback_data.Buf,
                callback_data.Buf + callback_data.SelectionEnd);
          if (callback_data.BufDirty) {
            IM_ASSERT(callback_data.BufTextLen ==
                      (int)strlen(callback_data.Buf)); // You need to maintain
                                                       // BufTextLen if you
                                                       // change the text!
            edit_state.CurLenW =
                ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size,
                                  callback_data.Buf, NULL);
            edit_state.CurLenA =
                callback_data.BufTextLen; // Assume correct length and valid
                                          // UTF-8 from user, saves us an extra
                                          // strlen()
            edit_state.CursorAnimReset();
          }
        }
      }

      // Copy back to user buffer
      if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0) {
        ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
        value_changed = true;
      }
    }
  }

  // Render
  // Select which buffer we are going to display. When
  // ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value.
  // We set buf to NULL to prevent accidental usage from now on.
  const char *buf_display =
      (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf;
  buf = NULL;

  if (!is_multiline)
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true,
                style.FrameRounding);

  const ImVec4 clip_rect(
      frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x,
      frame_bb.Min.y +
          size.y); // Not using frame_bb.Max because we have adjusted size
  ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos
                                   : frame_bb.Min + style.FramePadding;
  ImVec2 text_size(0.f, 0.f);
  const bool is_currently_scrolling =
      (edit_state.Id == id && is_multiline &&
       g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
  if (g.ActiveId == id || is_currently_scrolling) {
    edit_state.CursorAnim += io.DeltaTime;

    // This is going to be messy. We need to:
    // - Display the text (this alone can be more easily clipped)
    // - Handle scrolling, highlight selection, display cursor (those all
    // requires some form of 1d->2d cursor position calculation)
    // - Measure text height (for scrollbar)
    // We are attempting to do most of that in **one main pass** to minimize the
    // computation cost (non-negligible for large amount of text) + 2nd pass for
    // selection rendering (we could merge them by an extra refactoring effort)
    // FIXME: This should occur on buf_display but we'd need to maintain
    // cursor/select_start/select_end for UTF-8.
    const ImWchar *text_begin = edit_state.Text.Data;
    ImVec2 cursor_offset, select_start_offset;

    {
      // Count lines + find lines numbers straddling 'cursor' and 'select_start'
      // position.
      const ImWchar *searches_input_ptr[2];
      searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
      searches_input_ptr[1] = NULL;
      int searches_remaining = 1;
      int searches_result_line_number[2] = {-1, -999};
      if (edit_state.StbState.select_start != edit_state.StbState.select_end) {
        searches_input_ptr[1] =
            text_begin + ImMin(edit_state.StbState.select_start,
                               edit_state.StbState.select_end);
        searches_result_line_number[1] = -1;
        searches_remaining++;
      }

      // Iterate all lines to find our line numbers
      // In multi-line mode, we never exit the loop until all lines are counted,
      // so add one extra to the searches_remaining counter.
      searches_remaining += is_multiline ? 1 : 0;
      int line_count = 0;
      for (const ImWchar *s = text_begin; *s != 0; s++)
        if (*s == '\n') {
          line_count++;
          if (searches_result_line_number[0] == -1 &&
              s >= searches_input_ptr[0]) {
            searches_result_line_number[0] = line_count;
            if (--searches_remaining <= 0)
              break;
          }
          if (searches_result_line_number[1] == -1 &&
              s >= searches_input_ptr[1]) {
            searches_result_line_number[1] = line_count;
            if (--searches_remaining <= 0)
              break;
          }
        }
      line_count++;
      if (searches_result_line_number[0] == -1)
        searches_result_line_number[0] = line_count;
      if (searches_result_line_number[1] == -1)
        searches_result_line_number[1] = line_count;

      // Calculate 2d position by finding the beginning of the line and
      // measuring distance
      cursor_offset.x =
          InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin),
                                 searches_input_ptr[0])
              .x;
      cursor_offset.y = searches_result_line_number[0] * g.FontSize;
      if (searches_result_line_number[1] >= 0) {
        select_start_offset.x =
            InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin),
                                   searches_input_ptr[1])
                .x;
        select_start_offset.y = searches_result_line_number[1] * g.FontSize;
      }

      // Calculate text height
      if (is_multiline)
        text_size = ImVec2(size.x, line_count * g.FontSize);
    }

    // Scroll
    if (edit_state.CursorFollow) {
      // Horizontal scroll in chunks of quarter width
      if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll)) {
        const float scroll_increment_x = size.x * 0.25f;
        if (cursor_offset.x < edit_state.ScrollX)
          edit_state.ScrollX =
              (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
        else if (cursor_offset.x - size.x >= edit_state.ScrollX)
          edit_state.ScrollX =
              (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
      } else {
        edit_state.ScrollX = 0.0f;
      }

      // Vertical scroll
      if (is_multiline) {
        float scroll_y = draw_window->Scroll.y;
        if (cursor_offset.y - g.FontSize < scroll_y)
          scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
        else if (cursor_offset.y - size.y >= scroll_y)
          scroll_y = cursor_offset.y - size.y;
        draw_window->DC.CursorPos.y +=
            (draw_window->Scroll.y - scroll_y); // To avoid a frame of lag
        draw_window->Scroll.y = scroll_y;
        render_pos.y = draw_window->DC.CursorPos.y;
      }
    }
    edit_state.CursorFollow = false;
    const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

    // Draw selection
    if (edit_state.StbState.select_start != edit_state.StbState.select_end) {
      const ImWchar *text_selected_begin =
          text_begin + ImMin(edit_state.StbState.select_start,
                             edit_state.StbState.select_end);
      const ImWchar *text_selected_end =
          text_begin + ImMax(edit_state.StbState.select_start,
                             edit_state.StbState.select_end);

      float bg_offy_up =
          is_multiline ? 0.0f : -1.0f; // FIXME: those offsets should be part of
                                       // the style? they don't play so well
                                       // with multi-line selection.
      float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
      ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg);
      ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
      for (const ImWchar *p = text_selected_begin; p < text_selected_end;) {
        if (rect_pos.y > clip_rect.w + g.FontSize)
          break;
        if (rect_pos.y < clip_rect.y) {
          while (p < text_selected_end)
            if (*p++ == '\n')
              break;
        } else {
          ImVec2 rect_size =
              InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
          if (rect_size.x <= 0.0f)
            rect_size.x =
                (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') *
                             0.50f); // So we can see selected empty lines
          ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize),
                      rect_pos + ImVec2(rect_size.x, bg_offy_dn));
          rect.Clip(clip_rect);
          if (rect.Overlaps(clip_rect))
            draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
        }
        rect_pos.x = render_pos.x - render_scroll.x;
        rect_pos.y += g.FontSize;
      }
    }

    draw_window->DrawList->AddText(g.Font, g.FontSize,
                                   render_pos - render_scroll,
                                   GetColorU32(ImGuiCol_Text), buf_display,
                                   buf_display + edit_state.CurLenA, 0.0f,
                                   is_multiline ? NULL : &clip_rect);

    // Draw blinking cursor
    bool cursor_is_visible = (g.InputTextState.CursorAnim <= 0.0f) ||
                             fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
    ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
    ImRect cursor_screen_rect(
        cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f,
        cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
    if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
      draw_window->DrawList->AddLine(cursor_screen_rect.Min,
                                     cursor_screen_rect.GetBL(),
                                     GetColorU32(ImGuiCol_Text));

    // Notify OS of text input position for advanced IME (-1 x offset so that
    // Windows IME can cover our cursor. Bit of an extra nicety.)
    if (is_editable)
      g.OsImePosRequest =
          ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
  } else {
    // Render text only
    const char *buf_end = NULL;
    if (is_multiline)
      text_size = ImVec2(
          size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) *
                      g.FontSize); // We don't need width
    draw_window->DrawList->AddText(
        g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display,
        buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
  }

  if (is_multiline) {
    Dummy(text_size +
          ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
    EndChildFrame();
    EndGroup();
  }

  if (is_password)
    PopFont();

  // Log as text
  if (g.LogEnabled && !is_password)
    LogRenderedText(render_pos, buf_display, NULL);

  if (label_size.x > 0)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                      frame_bb.Min.y + style.FramePadding.y),
               label);

  if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
    return enter_pressed;
  else
    return value_changed;
}

bool ImGui::InputText(const char *label, char *buf, size_t buf_size,
                      ImGuiInputTextFlags flags, ImGuiTextEditCallback callback,
                      void *user_data) {
  IM_ASSERT(
      !(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
  return InputTextEx(label, buf, (int)buf_size, ImVec2(0, 0), flags, callback,
                     user_data);
}

bool ImGui::InputTextMultiline(const char *label, char *buf, size_t buf_size,
                               const ImVec2 &size, ImGuiInputTextFlags flags,
                               ImGuiTextEditCallback callback,
                               void *user_data) {
  return InputTextEx(label, buf, (int)buf_size, size,
                     flags | ImGuiInputTextFlags_Multiline, callback,
                     user_data);
}

// NB: scalar_format here must be a simple "%xx" format string with no
// prefix/suffix (unlike the Drag/Slider functions "display_format" argument)
bool ImGui::InputScalarEx(const char *label, ImGuiDataType data_type,
                          void *data_ptr, void *step_ptr, void *step_fast_ptr,
                          const char *scalar_format,
                          ImGuiInputTextFlags extra_flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  BeginGroup();
  PushID(label);
  const ImVec2 button_sz =
      ImVec2(g.FontSize, g.FontSize) + style.FramePadding * 2.0f;
  if (step_ptr)
    PushItemWidth(ImMax(
        1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x) * 2));

  char buf[64];
  DataTypeFormatString(data_type, data_ptr, scalar_format, buf,
                       IM_ARRAYSIZE(buf));

  bool value_changed = false;
  if (!(extra_flags & ImGuiInputTextFlags_CharsHexadecimal))
    extra_flags |= ImGuiInputTextFlags_CharsDecimal;
  extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
  if (InputText("", buf, IM_ARRAYSIZE(buf), extra_flags)) // PushId(label) + ""
                                                          // gives us the
                                                          // expected ID from
                                                          // outside point of
                                                          // view
    value_changed =
        DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(),
                                data_type, data_ptr, scalar_format);

  // Step buttons
  if (step_ptr) {
    PopItemWidth();
    SameLine(0, style.ItemInnerSpacing.x);
    if (ButtonEx("-", button_sz,
                 ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups)) {
      DataTypeApplyOp(data_type, '-', data_ptr,
                      g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
      value_changed = true;
    }
    SameLine(0, style.ItemInnerSpacing.x);
    if (ButtonEx("+", button_sz,
                 ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups)) {
      DataTypeApplyOp(data_type, '+', data_ptr,
                      g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
      value_changed = true;
    }
  }
  PopID();

  if (label_size.x > 0) {
    SameLine(0, style.ItemInnerSpacing.x);
    RenderText(ImVec2(window->DC.CursorPos.x,
                      window->DC.CursorPos.y + style.FramePadding.y),
               label);
    ItemSize(label_size, style.FramePadding.y);
  }
  EndGroup();

  return value_changed;
}

bool ImGui::InputFloat(const char *label, float *v, float step, float step_fast,
                       int decimal_precision, ImGuiInputTextFlags extra_flags) {
  char display_format[16];
  if (decimal_precision < 0)
    strcpy(display_format, "%f"); // Ideally we'd have a minimum decimal
                                  // precision of 1 to visually denote that this
                                  // is a float, while hiding non-significant
                                  // digits? %f doesn't have a minimum of 1
  else
    ImFormatString(display_format, IM_ARRAYSIZE(display_format), "%%.%df",
                   decimal_precision);
  return InputScalarEx(label, ImGuiDataType_Float, (void *)v,
                       (void *)(step > 0.0f ? &step : NULL),
                       (void *)(step_fast > 0.0f ? &step_fast : NULL),
                       display_format, extra_flags);
}

bool ImGui::InputInt(const char *label, int *v, int step, int step_fast,
                     ImGuiInputTextFlags extra_flags) {
  // Hexadecimal input provided as a convenience but the flag name is awkward.
  // Typically you'd use InputText() to parse your own data, if you want to
  // handle prefixes.
  const char *scalar_format =
      (extra_flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
  return InputScalarEx(label, ImGuiDataType_Int, (void *)v,
                       (void *)(step > 0.0f ? &step : NULL),
                       (void *)(step_fast > 0.0f ? &step_fast : NULL),
                       scalar_format, extra_flags);
}

bool ImGui::InputFloatN(const char *label, float *v, int components,
                        int decimal_precision,
                        ImGuiInputTextFlags extra_flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |=
        InputFloat("##v", &v[i], 0, 0, decimal_precision, extra_flags);
    SameLine(0, g.Style.ItemInnerSpacing.x);
    PopID();
    PopItemWidth();
  }
  PopID();

  window->DC.CurrentLineTextBaseOffset =
      ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

bool ImGui::InputFloat2(const char *label, float v[2], int decimal_precision,
                        ImGuiInputTextFlags extra_flags) {
  return InputFloatN(label, v, 2, decimal_precision, extra_flags);
}

bool ImGui::InputFloat3(const char *label, float v[3], int decimal_precision,
                        ImGuiInputTextFlags extra_flags) {
  return InputFloatN(label, v, 3, decimal_precision, extra_flags);
}

bool ImGui::InputFloat4(const char *label, float v[4], int decimal_precision,
                        ImGuiInputTextFlags extra_flags) {
  return InputFloatN(label, v, 4, decimal_precision, extra_flags);
}

bool ImGui::InputIntN(const char *label, int *v, int components,
                      ImGuiInputTextFlags extra_flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components);
  for (int i = 0; i < components; i++) {
    PushID(i);
    value_changed |= InputInt("##v", &v[i], 0, 0, extra_flags);
    SameLine(0, g.Style.ItemInnerSpacing.x);
    PopID();
    PopItemWidth();
  }
  PopID();

  window->DC.CurrentLineTextBaseOffset =
      ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
  TextUnformatted(label, FindRenderedTextEnd(label));
  EndGroup();

  return value_changed;
}

bool ImGui::InputInt2(const char *label, int v[2],
                      ImGuiInputTextFlags extra_flags) {
  return InputIntN(label, v, 2, extra_flags);
}

bool ImGui::InputInt3(const char *label, int v[3],
                      ImGuiInputTextFlags extra_flags) {
  return InputIntN(label, v, 3, extra_flags);
}

bool ImGui::InputInt4(const char *label, int v[4],
                      ImGuiInputTextFlags extra_flags) {
  return InputIntN(label, v, 4, extra_flags);
}

static bool Items_ArrayGetter(void *data, int idx, const char **out_text) {
  const char *const *items = (const char *const *)data;
  if (out_text)
    *out_text = items[idx];
  return true;
}

static bool Items_SingleStringGetter(void *data, int idx,
                                     const char **out_text) {
  // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1
  // active combo means the waste is limited.
  const char *items_separated_by_zeros = (const char *)data;
  int items_count = 0;
  const char *p = items_separated_by_zeros;
  while (*p) {
    if (idx == items_count)
      break;
    p += strlen(p) + 1;
    items_count++;
  }
  if (!*p)
    return false;
  if (out_text)
    *out_text = p;
  return true;
}

// Combo box helper allowing to pass an array of strings.
bool ImGui::Combo(const char *label, int *current_item,
                  const char *const *items, int items_count,
                  int height_in_items) {
  const bool value_changed = Combo(label, current_item, Items_ArrayGetter,
                                   (void *)items, items_count, height_in_items);
  return value_changed;
}

// Combo box helper allowing to pass all items in a single string.
bool ImGui::Combo(const char *label, int *current_item,
                  const char *items_separated_by_zeros, int height_in_items) {
  int items_count = 0;
  const char *p = items_separated_by_zeros; // FIXME-OPT: Avoid computing this,
                                            // or at least only when combo is
                                            // open
  while (*p) {
    p += strlen(p) + 1;
    items_count++;
  }
  bool value_changed =
      Combo(label, current_item, Items_SingleStringGetter,
            (void *)items_separated_by_zeros, items_count, height_in_items);
  return value_changed;
}

// Combo box function.
bool ImGui::Combo(const char *label, int *current_item,
                  bool (*items_getter)(void *, int, const char **), void *data,
                  int items_count, int height_in_items) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
>>>>>>> master
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w = CalcItemWidth();

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect frame_bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max +
                            ImVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                                   0.0f));
  ItemSize(total_bb, style.FramePadding.y);
  if (!ItemAdd(total_bb, &id))
    return false;

  const float arrow_size = (g.FontSize + style.FramePadding.x * 2.0f);
  const bool hovered = IsHovered(frame_bb, id);
  bool popup_open = IsPopupOpen(id);
  bool popup_opened_now = false;

  const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
  RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true,
              style.FrameRounding);
  RenderFrame(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y), frame_bb.Max,
              GetColorU32(popup_open || hovered ? ImGuiCol_ButtonHovered
                                                : ImGuiCol_Button),
              true,
              style.FrameRounding); // FIXME-ROUNDING
  RenderCollapseTriangle(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y) +
                             style.FramePadding,
                         true);

  if (*current_item >= 0 && *current_item < items_count) {
    const char *item_text;
    if (items_getter(data, *current_item, &item_text))
      RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max,
                        item_text, NULL, NULL, ImVec2(0.0f, 0.0f));
  }

  if (label_size.x > 0)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                      frame_bb.Min.y + style.FramePadding.y),
               label);

  if (hovered) {
    SetHoveredID(id);
    if (g.IO.MouseClicked[0]) {
      ClearActiveID();
      if (IsPopupOpen(id)) {
        ClosePopup(id);
      } else {
        FocusWindow(window);
        OpenPopup(label);
        popup_open = popup_opened_now = true;
      }
    }
  }

  bool value_changed = false;
  if (IsPopupOpen(id)) {
    // Size default to hold ~7 items
    if (height_in_items < 0)
      height_in_items = 7;

    float popup_height = (label_size.y + style.ItemSpacing.y) *
                             ImMin(items_count, height_in_items) +
                         (style.FramePadding.y * 3);
    float popup_y1 = frame_bb.Max.y;
    float popup_y2 =
        ImClamp(popup_y1 + popup_height, popup_y1,
                g.IO.DisplaySize.y - style.DisplaySafeAreaPadding.y);
    if ((popup_y2 - popup_y1) <
        ImMin(popup_height, frame_bb.Min.y - style.DisplaySafeAreaPadding.y)) {
      // Position our combo ABOVE because there's more space to fit! (FIXME:
      // Handle in Begin() or use a shared helper. We have similar code in
      // Begin() for popup placement)
      popup_y1 = ImClamp(frame_bb.Min.y - popup_height,
                         style.DisplaySafeAreaPadding.y, frame_bb.Min.y);
      popup_y2 = frame_bb.Min.y;
    }
    ImRect popup_rect(ImVec2(frame_bb.Min.x, popup_y1),
                      ImVec2(frame_bb.Max.x, popup_y2));
    SetNextWindowPos(popup_rect.Min);
    SetNextWindowSize(popup_rect.GetSize());
    PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);

    const ImGuiWindowFlags flags =
        ImGuiWindowFlags_ComboBox |
        ((window->Flags & ImGuiWindowFlags_ShowBorders)
             ? ImGuiWindowFlags_ShowBorders
             : 0);
    if (BeginPopupEx(label, flags)) {
      // Display items
      Spacing();
      for (int i = 0; i < items_count; i++) {
        PushID((void *)(intptr_t)i);
        const bool item_selected = (i == *current_item);
        const char *item_text;
        if (!items_getter(data, i, &item_text))
          item_text = "*Unknown item*";
        if (Selectable(item_text, item_selected)) {
          ClearActiveID();
          value_changed = true;
          *current_item = i;
        }
        if (item_selected && popup_opened_now)
          SetScrollHere();
        PopID();
      }
      EndPopup();
    }
    PopStyleVar();
  }
  return value_changed;
}

// Tip: pass an empty label (e.g. "##dummy") then you can use the space to draw
// other text or image.
// But you need to make sure the ID is unique, e.g. enclose calls in
// PushID/PopID.
bool ImGui::Selectable(const char *label, bool selected,
                       ImGuiSelectableFlags flags, const ImVec2 &size_arg) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  if ((flags & ImGuiSelectableFlags_SpanAllColumns) &&
      window->DC.ColumnsCount > 1)
    PopClipRect();

  ImGuiID id = window->GetID(label);
  ImVec2 label_size = CalcTextSize(label, NULL, true);
  ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x,
              size_arg.y != 0.0f ? size_arg.y : label_size.y);
  ImVec2 pos = window->DC.CursorPos;
  pos.y += window->DC.CurrentLineTextBaseOffset;
  ImRect bb(pos, pos + size);
  ItemSize(bb);

  // Fill horizontal space.
  ImVec2 window_padding = window->WindowPadding;
  float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns)
                    ? GetWindowContentRegionMax().x
                    : GetContentRegionMax().x;
  float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x -
                                         window->DC.CursorPos.x);
  ImVec2 size_draw(
      (size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth))
          ? size_arg.x
          : w_draw,
      size_arg.y != 0.0f ? size_arg.y : size.y);
  ImRect bb_with_spacing(pos, pos + size_draw);
  if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
    bb_with_spacing.Max.x += window_padding.x;

  // Selectables are tightly packed together, we extend the box to cover spacing
  // between selectable.
  float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
  float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
  float spacing_R = style.ItemSpacing.x - spacing_L;
  float spacing_D = style.ItemSpacing.y - spacing_U;
  bb_with_spacing.Min.x -= spacing_L;
  bb_with_spacing.Min.y -= spacing_U;
  bb_with_spacing.Max.x += spacing_R;
  bb_with_spacing.Max.y += spacing_D;
  if (!ItemAdd(bb_with_spacing, &id)) {
    if ((flags & ImGuiSelectableFlags_SpanAllColumns) &&
        window->DC.ColumnsCount > 1)
      PushColumnClipRect();
    return false;
  }

  ImGuiButtonFlags button_flags = 0;
  if (flags & ImGuiSelectableFlags_Menu)
    button_flags |= ImGuiButtonFlags_PressedOnClick;
  if (flags & ImGuiSelectableFlags_MenuItem)
    button_flags |=
        ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease;
  if (flags & ImGuiSelectableFlags_Disabled)
    button_flags |= ImGuiButtonFlags_Disabled;
  if (flags & ImGuiSelectableFlags_AllowDoubleClick)
    button_flags |= ImGuiButtonFlags_PressedOnClickRelease |
                    ImGuiButtonFlags_PressedOnDoubleClick;
  bool hovered, held;
  bool pressed =
      ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
  if (flags & ImGuiSelectableFlags_Disabled)
    selected = false;

  // Render
  if (hovered || selected) {
    const ImU32 col = GetColorU32(
        (held && hovered) ? ImGuiCol_HeaderActive
                          : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
    RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
  }

  if ((flags & ImGuiSelectableFlags_SpanAllColumns) &&
      window->DC.ColumnsCount > 1) {
    PushColumnClipRect();
    bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
  }

  if (flags & ImGuiSelectableFlags_Disabled)
    PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
  RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size,
                    ImVec2(0.0f, 0.0f));
  if (flags & ImGuiSelectableFlags_Disabled)
    PopStyleColor();

  // Automatically close popups
  if (pressed && !(flags & ImGuiSelectableFlags_DontClosePopups) &&
      (window->Flags & ImGuiWindowFlags_Popup))
    CloseCurrentPopup();
  return pressed;
}

bool ImGui::Selectable(const char *label, bool *p_selected,
                       ImGuiSelectableFlags flags, const ImVec2 &size_arg) {
  if (Selectable(label, *p_selected, flags, size_arg)) {
    *p_selected = !*p_selected;
    return true;
  }
  return false;
}

// Helper to calculate the size of a listbox and display a label on the right.
// Tip: To have a list filling the entire window width, PushItemWidth(-1) and
// pass an empty label "##empty"
<<<<<<< HEAD
bool ImGui::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    const ImGuiStyle& style      = GetStyle();
    const ImGuiID     id         = GetID(label);
    const ImVec2      label_size = CalcTextSize(label, NULL, true);

    // Size default to hold ~7 items. Fractional number of items helps seeing that
    // we can scroll down/up without looking at scrollbar.
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
    ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    ImRect bb(frame_bb.Min,
        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    window->DC.LastItemRect = bb;

    BeginGroup();
    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    BeginChildFrame(id, frame_bb.GetSize());
    return true;
}

bool ImGui::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
    // Size default to hold ~7 items. Fractional number of items helps seeing that
    // we can scroll down/up without looking at scrollbar.
    // However we don't add +0.40f if items_count <= height_in_items. It is
    // slightly dodgy, because it means a dynamic list of items will make the
    // widget resize occasionally when it crosses that size.
    // I am expecting that someone will come and complain about this behavior in a
    // remote future, then we can advise on a better solution.
    if (height_in_items < 0) height_in_items = ImMin(items_count, 7);
    float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

    // We include ItemSpacing.y so that a list sized for the exact number of items
    // doesn't make a scrollbar appears. We could also enforce that by passing a
    // flag to BeginChild().
    ImVec2 size;
    size.x = 0.0f;
    size.y = GetTextLineHeightWithSpacing() * height_in_items_f + GetStyle().ItemSpacing.y;
    return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter()
{
    ImGuiWindow*      parent_window = GetParentWindow();
    const ImRect      bb            = parent_window->DC.LastItemRect;
    const ImGuiStyle& style         = GetStyle();

    EndChildFrame();

    // Redeclare item size so that it includes the label (we have stored the full
    // size in LastItemRect)
    // We call SameLine() to restore DC.CurrentLine* data
    SameLine();
    parent_window->DC.CursorPos = bb.Min;
    ItemSize(bb, style.FramePadding.y);
    EndGroup();
}

bool ImGui::ListBox(const char* label, int* current_item, const char* const* items, int items_count, int height_items)
{
    const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
    return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data,
    int items_count, int height_in_items)
{
    if (!ListBoxHeader(label, items_count, height_in_items)) return false;

    // Assume all items have even height (= 1 line of text). If you need items of
    // different or variable sizes you can create a custom version of ListBox() in
    // your code without using the clipper.
    bool             value_changed = false;
    ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing());
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            const bool  item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text)) item_text = "*Unknown item*";

            PushID(i);
            if (Selectable(item_text, item_selected)) {
                *current_item = i;
                value_changed = true;
            }
            PopID();
        }
    ListBoxFooter();
    return value_changed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g             = *GImGui;
    ImVec2        pos           = window->DC.CursorPos;
    ImVec2        label_size    = CalcTextSize(label, NULL, true);
    ImVec2        shortcut_size = shortcut ? CalcTextSize(shortcut, NULL) : ImVec2(0.0f, 0.0f);
    float         w             = window->MenuColumns.DeclColumns(label_size.x,
        shortcut_size.x,
        (float)(int)(g.FontSize * 1.20f));  // Feedback for next frame
    float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);

    bool pressed = Selectable(label,
        false,
        ImGuiSelectableFlags_MenuItem | ImGuiSelectableFlags_DrawFillAvailWidth |
            (enabled ? 0 : ImGuiSelectableFlags_Disabled),
        ImVec2(w, 0.0f));
    if (shortcut_size.x > 0.0f) {
        PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
        RenderText(pos + ImVec2(window->MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
        PopStyleColor();
    }

    if (selected)
        RenderCheckMark(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f, 0.0f),
            GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled));

    return pressed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
    if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled)) {
        if (p_selected) *p_selected = !*p_selected;
        return true;
    }
=======
bool ImGui::ListBoxHeader(const char *label, const ImVec2 &size_arg) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
>>>>>>> master
    return false;

  const ImGuiStyle &style = GetStyle();
  const ImGuiID id = GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  // Size default to hold ~7 items. Fractional number of items helps seeing that
  // we can scroll down/up without looking at scrollbar.
  ImVec2 size =
      CalcItemSize(size_arg, CalcItemWidth(),
                   GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
  ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
  ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
  ImRect bb(frame_bb.Min,
            frame_bb.Max + ImVec2(label_size.x > 0.0f
                                      ? style.ItemInnerSpacing.x + label_size.x
                                      : 0.0f,
                                  0.0f));
  window->DC.LastItemRect = bb;

  BeginGroup();
  if (label_size.x > 0)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                      frame_bb.Min.y + style.FramePadding.y),
               label);

  BeginChildFrame(id, frame_bb.GetSize());
  return true;
}

<<<<<<< HEAD
bool ImGui::BeginMainMenuBar()
{
    ImGuiContext& g = *GImGui;
    SetNextWindowPos(ImVec2(0.0f, 0.0f));
    SetNextWindowSize(ImVec2(g.IO.DisplaySize.x, g.FontBaseSize + g.Style.FramePadding.y * 2.0f));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
    if (!Begin("##MainMenuBar",
            NULL,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar) ||
        !BeginMenuBar())
    {
        End();
        PopStyleVar(2);
        return false;
=======
bool ImGui::ListBoxHeader(const char *label, int items_count,
                          int height_in_items) {
  // Size default to hold ~7 items. Fractional number of items helps seeing that
  // we can scroll down/up without looking at scrollbar.
  // However we don't add +0.40f if items_count <= height_in_items. It is
  // slightly dodgy, because it means a dynamic list of items will make the
  // widget resize occasionally when it crosses that size.
  // I am expecting that someone will come and complain about this behavior in a
  // remote future, then we can advise on a better solution.
  if (height_in_items < 0)
    height_in_items = ImMin(items_count, 7);
  float height_in_items_f = height_in_items < items_count
                                ? (height_in_items + 0.40f)
                                : (height_in_items + 0.00f);

  // We include ItemSpacing.y so that a list sized for the exact number of items
  // doesn't make a scrollbar appears. We could also enforce that by passing a
  // flag to BeginChild().
  ImVec2 size;
  size.x = 0.0f;
  size.y = GetTextLineHeightWithSpacing() * height_in_items_f +
           GetStyle().ItemSpacing.y;
  return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter() {
  ImGuiWindow *parent_window = GetParentWindow();
  const ImRect bb = parent_window->DC.LastItemRect;
  const ImGuiStyle &style = GetStyle();

  EndChildFrame();

  // Redeclare item size so that it includes the label (we have stored the full
  // size in LastItemRect)
  // We call SameLine() to restore DC.CurrentLine* data
  SameLine();
  parent_window->DC.CursorPos = bb.Min;
  ItemSize(bb, style.FramePadding.y);
  EndGroup();
}

bool ImGui::ListBox(const char *label, int *current_item,
                    const char *const *items, int items_count,
                    int height_items) {
  const bool value_changed = ListBox(label, current_item, Items_ArrayGetter,
                                     (void *)items, items_count, height_items);
  return value_changed;
}

bool ImGui::ListBox(const char *label, int *current_item,
                    bool (*items_getter)(void *, int, const char **),
                    void *data, int items_count, int height_in_items) {
  if (!ListBoxHeader(label, items_count, height_in_items))
    return false;

  // Assume all items have even height (= 1 line of text). If you need items of
  // different or variable sizes you can create a custom version of ListBox() in
  // your code without using the clipper.
  bool value_changed = false;
  ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing());
  while (clipper.Step())
    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
      const bool item_selected = (i == *current_item);
      const char *item_text;
      if (!items_getter(data, i, &item_text))
        item_text = "*Unknown item*";

      PushID(i);
      if (Selectable(item_text, item_selected)) {
        *current_item = i;
        value_changed = true;
      }
      PopID();
>>>>>>> master
    }
  ListBoxFooter();
  return value_changed;
}

bool ImGui::MenuItem(const char *label, const char *shortcut, bool selected,
                     bool enabled) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  ImVec2 pos = window->DC.CursorPos;
  ImVec2 label_size = CalcTextSize(label, NULL, true);
  ImVec2 shortcut_size =
      shortcut ? CalcTextSize(shortcut, NULL) : ImVec2(0.0f, 0.0f);
  float w = window->MenuColumns.DeclColumns(
      label_size.x, shortcut_size.x,
      (float)(int)(g.FontSize * 1.20f)); // Feedback for next frame
  float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);

  bool pressed = Selectable(label, false,
                            ImGuiSelectableFlags_MenuItem |
                                ImGuiSelectableFlags_DrawFillAvailWidth |
                                (enabled ? 0 : ImGuiSelectableFlags_Disabled),
                            ImVec2(w, 0.0f));
  if (shortcut_size.x > 0.0f) {
    PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    RenderText(pos + ImVec2(window->MenuColumns.Pos[1] + extra_w, 0.0f),
               shortcut, NULL, false);
    PopStyleColor();
  }

  if (selected)
    RenderCheckMark(
        pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f,
                     0.0f),
        GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled));

  return pressed;
}

bool ImGui::MenuItem(const char *label, const char *shortcut, bool *p_selected,
                     bool enabled) {
  if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled)) {
    if (p_selected)
      *p_selected = !*p_selected;
    return true;
  }
  return false;
}

bool ImGui::BeginMainMenuBar() {
  ImGuiContext &g = *GImGui;
  SetNextWindowPos(ImVec2(0.0f, 0.0f));
  SetNextWindowSize(ImVec2(g.IO.DisplaySize.x,
                           g.FontBaseSize + g.Style.FramePadding.y * 2.0f));
  PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
  if (!Begin("##MainMenuBar", NULL,
             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar) ||
      !BeginMenuBar()) {
    End();
    PopStyleVar(2);
    return false;
  }
  g.CurrentWindow->DC.MenuBarOffsetX += g.Style.DisplaySafeAreaPadding.x;
  return true;
}

<<<<<<< HEAD
bool ImGui::BeginMenuBar()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;
    if (!(window->Flags & ImGuiWindowFlags_MenuBar)) return false;

    IM_ASSERT(!window->DC.MenuBarAppending);
    BeginGroup();  // Save position
    PushID("##menubar");
    ImRect rect = window->MenuBarRect();
    PushClipRect(ImVec2(ImFloor(rect.Min.x + 0.5f), ImFloor(rect.Min.y + window->BorderSize + 0.5f)),
        ImVec2(ImFloor(rect.Max.x + 0.5f), ImFloor(rect.Max.y + 0.5f)),
        false);
    window->DC.CursorPos  = ImVec2(rect.Min.x + window->DC.MenuBarOffsetX, rect.Min.y);  // + g.Style.FramePadding.y);
    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.MenuBarAppending = true;
    AlignFirstTextHeightToWidgets();
    return true;
}

void ImGui::EndMenuBar()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
    IM_ASSERT(window->DC.MenuBarAppending);
    PopClipRect();
    PopID();
    window->DC.MenuBarOffsetX                  = window->DC.CursorPos.x - window->MenuBarRect().Min.x;
    window->DC.GroupStack.back().AdvanceCursor = false;
    EndGroup();
    window->DC.LayoutType       = ImGuiLayoutType_Vertical;
    window->DC.MenuBarAppending = false;
}

bool ImGui::BeginMenu(const char* label, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);

    ImVec2       label_size            = CalcTextSize(label, NULL, true);
    ImGuiWindow* backed_focused_window = g.FocusedWindow;

    bool pressed;
    bool menu_is_open    = IsPopupOpen(id);
    bool menuset_is_open = !(window->Flags & ImGuiWindowFlags_Popup) &&
                           (g.OpenPopupStack.Size > g.CurrentPopupStack.Size &&
                               g.OpenPopupStack[g.CurrentPopupStack.Size].ParentMenuSet == window->GetID("##menus"));
    if (menuset_is_open) g.FocusedWindow = window;

    // The reference position stored in popup_pos will be used by Begin() to find
    // a suitable position for the child menu (using FindBestPopupWindowPos).
    ImVec2 popup_pos, pos = window->DC.CursorPos;
    if (window->DC.LayoutType == ImGuiLayoutType_Horizontal) {
        popup_pos = ImVec2(pos.x - window->WindowPadding.x, pos.y - style.FramePadding.y + window->MenuBarHeight());
        window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
        PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
        float w = label_size.x;
        pressed = Selectable(label,
            menu_is_open,
            ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups |
                (!enabled ? ImGuiSelectableFlags_Disabled : 0),
            ImVec2(w, 0.0f));
        PopStyleVar();
        SameLine();
        window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
    }
    else
    {
        popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
        float w   = window->MenuColumns.DeclColumns(label_size.x,
            0.0f,
            (float)(int)(g.FontSize * 1.20f));  // Feedback to next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
        pressed       = Selectable(label,
            menu_is_open,
            ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | ImGuiSelectableFlags_DrawFillAvailWidth |
                (!enabled ? ImGuiSelectableFlags_Disabled : 0),
            ImVec2(w, 0.0f));
        if (!enabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
        RenderCollapseTriangle(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f, 0.0f), false);
        if (!enabled) PopStyleColor();
    }

    bool hovered                         = enabled && IsHovered(window->DC.LastItemRect, id);
    if (menuset_is_open) g.FocusedWindow = backed_focused_window;

    bool want_open = false, want_close = false;
    if (window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) {
        // Implement
        // http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to
        // avoid using timers, so menus feels more reactive.
        bool moving_within_opened_triangle = false;
        if (g.HoveredWindow == window && g.OpenPopupStack.Size > g.CurrentPopupStack.Size &&
            g.OpenPopupStack[g.CurrentPopupStack.Size].ParentWindow == window)
        {
            if (ImGuiWindow* next_window = g.OpenPopupStack[g.CurrentPopupStack.Size].Window) {
                ImRect next_window_rect = next_window->Rect();
                ImVec2 ta               = g.IO.MousePos - g.IO.MouseDelta;
                ImVec2 tb = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
                ImVec2 tc = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
                float  extra = ImClamp(fabsf(ta.x - tb.x) * 0.30f, 5.0f, 30.0f);  // add a bit of extra slack.
                ta.x += (window->Pos.x < next_window->Pos.x) ? -0.5f : +0.5f;     // to avoid numerical issues
                tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);  // triangle is maximum 200 high to limit
                                                                      // the slope and the bias toward large
                                                                      // sub-menus // FIXME: Multiply by
                                                                      // fb_scale?
                tc.y                          = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
                moving_within_opened_triangle = ImIsPointInTriangle(g.IO.MousePos, ta, tb, tc);
                // window->DrawList->PushClipRectFullScreen();
                // window->DrawList->AddTriangleFilled(ta, tb, tc,
                // moving_within_opened_triangle ? IM_COL32(0,128,0,128) :
                // IM_COL32(128,0,0,128)); window->DrawList->PopClipRect(); // Debug
            }
        }

        want_close = (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 &&
                      g.HoveredIdPreviousFrame != id && !moving_within_opened_triangle);
        want_open =
            (!menu_is_open && hovered && !moving_within_opened_triangle) || (!menu_is_open && hovered && pressed);
    }
    else if (menu_is_open && pressed && menuset_is_open)  // menu-bar: click open menu to close
    {
        want_close = true;
        want_open = menu_is_open = false;
    }
    else if (pressed || (hovered && menuset_is_open && !menu_is_open))  // menu-bar: first click to open, then
                                                                        // hover to open others
        want_open = true;
    if (!enabled)  // explicitly close if an open menu becomes disabled, facilitate
                   // users code a lot in pattern such as 'if (BeginMenu("options",
                   // has_object)) { ..use object.. }'
        want_close = true;
    if (want_close && IsPopupOpen(id)) ClosePopupToLevel(GImGui->CurrentPopupStack.Size);

    if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.CurrentPopupStack.Size) {
        // Don't recycle same menu level in the same frame, first close the other
        // menu and yield for a frame.
        OpenPopup(label);
        return false;
    }

    menu_is_open |= want_open;
    if (want_open) OpenPopup(label);

    if (menu_is_open) {
        SetNextWindowPos(popup_pos, ImGuiSetCond_Always);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_ShowBorders | ((window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu))
                                                   ? ImGuiWindowFlags_ChildMenu | ImGuiWindowFlags_ChildWindow
                                                   : ImGuiWindowFlags_ChildMenu);
        menu_is_open = BeginPopupEx(label, flags);  // menu_is_open can be 'false'
                                                    // when the popup is completely
                                                    // clipped (e.g. zero size
                                                    // display)
    }

    return menu_is_open;
}

void ImGui::EndMenu() { EndPopup(); }
=======
void ImGui::EndMainMenuBar() {
  EndMenuBar();
  End();
  PopStyleVar(2);
}

bool ImGui::BeginMenuBar() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;
  if (!(window->Flags & ImGuiWindowFlags_MenuBar))
    return false;

  IM_ASSERT(!window->DC.MenuBarAppending);
  BeginGroup(); // Save position
  PushID("##menubar");
  ImRect rect = window->MenuBarRect();
  PushClipRect(ImVec2(ImFloor(rect.Min.x + 0.5f),
                      ImFloor(rect.Min.y + window->BorderSize + 0.5f)),
               ImVec2(ImFloor(rect.Max.x + 0.5f), ImFloor(rect.Max.y + 0.5f)),
               false);
  window->DC.CursorPos = ImVec2(rect.Min.x + window->DC.MenuBarOffsetX,
                                rect.Min.y); // + g.Style.FramePadding.y);
  window->DC.LayoutType = ImGuiLayoutType_Horizontal;
  window->DC.MenuBarAppending = true;
  AlignFirstTextHeightToWidgets();
  return true;
}

void ImGui::EndMenuBar() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
  IM_ASSERT(window->DC.MenuBarAppending);
  PopClipRect();
  PopID();
  window->DC.MenuBarOffsetX =
      window->DC.CursorPos.x - window->MenuBarRect().Min.x;
  window->DC.GroupStack.back().AdvanceCursor = false;
  EndGroup();
  window->DC.LayoutType = ImGuiLayoutType_Vertical;
  window->DC.MenuBarAppending = false;
}

bool ImGui::BeginMenu(const char *label, bool enabled) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);

  ImVec2 label_size = CalcTextSize(label, NULL, true);
  ImGuiWindow *backed_focused_window = g.FocusedWindow;

  bool pressed;
  bool menu_is_open = IsPopupOpen(id);
  bool menuset_is_open =
      !(window->Flags & ImGuiWindowFlags_Popup) &&
      (g.OpenPopupStack.Size > g.CurrentPopupStack.Size &&
       g.OpenPopupStack[g.CurrentPopupStack.Size].ParentMenuSet ==
           window->GetID("##menus"));
  if (menuset_is_open)
    g.FocusedWindow = window;

  // The reference position stored in popup_pos will be used by Begin() to find
  // a suitable position for the child menu (using FindBestPopupWindowPos).
  ImVec2 popup_pos, pos = window->DC.CursorPos;
  if (window->DC.LayoutType == ImGuiLayoutType_Horizontal) {
    popup_pos = ImVec2(pos.x - window->WindowPadding.x,
                       pos.y - style.FramePadding.y + window->MenuBarHeight());
    window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
    PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
    float w = label_size.x;
    pressed = Selectable(label, menu_is_open,
                         ImGuiSelectableFlags_Menu |
                             ImGuiSelectableFlags_DontClosePopups |
                             (!enabled ? ImGuiSelectableFlags_Disabled : 0),
                         ImVec2(w, 0.0f));
    PopStyleVar();
    SameLine();
    window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
  } else {
    popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
    float w = window->MenuColumns.DeclColumns(
        label_size.x, 0.0f,
        (float)(int)(g.FontSize * 1.20f)); // Feedback to next frame
    float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
    pressed = Selectable(label, menu_is_open,
                         ImGuiSelectableFlags_Menu |
                             ImGuiSelectableFlags_DontClosePopups |
                             ImGuiSelectableFlags_DrawFillAvailWidth |
                             (!enabled ? ImGuiSelectableFlags_Disabled : 0),
                         ImVec2(w, 0.0f));
    if (!enabled)
      PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    RenderCollapseTriangle(
        pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f,
                     0.0f),
        false);
    if (!enabled)
      PopStyleColor();
  }

  bool hovered = enabled && IsHovered(window->DC.LastItemRect, id);
  if (menuset_is_open)
    g.FocusedWindow = backed_focused_window;

  bool want_open = false, want_close = false;
  if (window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) {
    // Implement
    // http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to
    // avoid using timers, so menus feels more reactive.
    bool moving_within_opened_triangle = false;
    if (g.HoveredWindow == window &&
        g.OpenPopupStack.Size > g.CurrentPopupStack.Size &&
        g.OpenPopupStack[g.CurrentPopupStack.Size].ParentWindow == window) {
      if (ImGuiWindow *next_window =
              g.OpenPopupStack[g.CurrentPopupStack.Size].Window) {
        ImRect next_window_rect = next_window->Rect();
        ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
        ImVec2 tb = (window->Pos.x < next_window->Pos.x)
                        ? next_window_rect.GetTL()
                        : next_window_rect.GetTR();
        ImVec2 tc = (window->Pos.x < next_window->Pos.x)
                        ? next_window_rect.GetBL()
                        : next_window_rect.GetBR();
        float extra = ImClamp(fabsf(ta.x - tb.x) * 0.30f, 5.0f,
                              30.0f); // add a bit of extra slack.
        ta.x += (window->Pos.x < next_window->Pos.x)
                    ? -0.5f
                    : +0.5f; // to avoid numerical issues
        tb.y = ta.y + ImMax((tb.y - extra) - ta.y,
                            -100.0f); // triangle is maximum 200 high to limit
                                      // the slope and the bias toward large
                                      // sub-menus // FIXME: Multiply by
                                      // fb_scale?
        tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
        moving_within_opened_triangle =
            ImIsPointInTriangle(g.IO.MousePos, ta, tb, tc);
        // window->DrawList->PushClipRectFullScreen();
        // window->DrawList->AddTriangleFilled(ta, tb, tc,
        // moving_within_opened_triangle ? IM_COL32(0,128,0,128) :
        // IM_COL32(128,0,0,128)); window->DrawList->PopClipRect(); // Debug
      }
    }

    want_close =
        (menu_is_open && !hovered && g.HoveredWindow == window &&
         g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id &&
         !moving_within_opened_triangle);
    want_open = (!menu_is_open && hovered && !moving_within_opened_triangle) ||
                (!menu_is_open && hovered && pressed);
  } else if (menu_is_open && pressed &&
             menuset_is_open) // menu-bar: click open menu to close
  {
    want_close = true;
    want_open = menu_is_open = false;
  } else if (pressed || (hovered && menuset_is_open &&
                         !menu_is_open)) // menu-bar: first click to open, then
                                         // hover to open others
    want_open = true;
  if (!enabled) // explicitly close if an open menu becomes disabled, facilitate
                // users code a lot in pattern such as 'if (BeginMenu("options",
                // has_object)) { ..use object.. }'
    want_close = true;
  if (want_close && IsPopupOpen(id))
    ClosePopupToLevel(GImGui->CurrentPopupStack.Size);

  if (!menu_is_open && want_open &&
      g.OpenPopupStack.Size > g.CurrentPopupStack.Size) {
    // Don't recycle same menu level in the same frame, first close the other
    // menu and yield for a frame.
    OpenPopup(label);
    return false;
  }

  menu_is_open |= want_open;
  if (want_open)
    OpenPopup(label);

  if (menu_is_open) {
    SetNextWindowPos(popup_pos, ImGuiSetCond_Always);
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_ShowBorders |
        ((window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu))
             ? ImGuiWindowFlags_ChildMenu | ImGuiWindowFlags_ChildWindow
             : ImGuiWindowFlags_ChildMenu);
    menu_is_open = BeginPopupEx(label, flags); // menu_is_open can be 'false'
                                               // when the popup is completely
                                               // clipped (e.g. zero size
                                               // display)
  }

  return menu_is_open;
}
>>>>>>> master

void ImGui::EndMenu() { EndPopup(); }

// A little colored square. Return true when clicked.
// FIXME: May want to display/ignore the alpha component in the color display?
// Yet show it in the tooltip.
<<<<<<< HEAD
bool ImGui::ColorButton(const ImVec4& col, bool small_height, bool outline_border)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g           = *GImGui;
    const ImGuiStyle& style       = g.Style;
    const ImGuiID     id          = window->GetID("#colorbutton");
    const float       square_size = g.FontSize;
    const ImRect      bb(window->DC.CursorPos,
        window->DC.CursorPos + ImVec2(square_size + style.FramePadding.y * 2,
                                   square_size + (small_height ? 0 : style.FramePadding.y * 2)));
    ItemSize(bb, small_height ? 0.0f : style.FramePadding.y);
    if (!ItemAdd(bb, &id)) return false;
=======
bool ImGui::ColorButton(const ImVec4 &col, bool small_height,
                        bool outline_border) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID("#colorbutton");
  const float square_size = g.FontSize;
  const ImRect bb(
      window->DC.CursorPos,
      window->DC.CursorPos +
          ImVec2(square_size + style.FramePadding.y * 2,
                 square_size + (small_height ? 0 : style.FramePadding.y * 2)));
  ItemSize(bb, small_height ? 0.0f : style.FramePadding.y);
  if (!ItemAdd(bb, &id))
    return false;
>>>>>>> master

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held);
  RenderFrame(bb.Min, bb.Max, GetColorU32(col), outline_border,
              style.FrameRounding);

<<<<<<< HEAD
    if (hovered)
        SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X",
            col.x,
            col.y,
            col.z,
            col.w,
            IM_F32_TO_INT8_SAT(col.x),
            IM_F32_TO_INT8_SAT(col.y),
            IM_F32_TO_INT8_SAT(col.z),
            IM_F32_TO_INT8_SAT(col.w));
=======
  if (hovered)
    SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col.x, col.y,
               col.z, col.w, IM_F32_TO_INT8_SAT(col.x),
               IM_F32_TO_INT8_SAT(col.y), IM_F32_TO_INT8_SAT(col.z),
               IM_F32_TO_INT8_SAT(col.w));
>>>>>>> master

  return pressed;
}

<<<<<<< HEAD
bool ImGui::ColorEdit3(const char* label, float col[3])
{
    float col4[4];
    col4[0]                  = col[0];
    col4[1]                  = col[1];
    col4[2]                  = col[2];
    col4[3]                  = 1.0f;
    const bool value_changed = ColorEdit4(label, col4, false);
    col[0]                   = col4[0];
    col[1]                   = col4[1];
    col[2]                   = col4[2];
    return value_changed;
=======
bool ImGui::ColorEdit3(const char *label, float col[3]) {
  float col4[4];
  col4[0] = col[0];
  col4[1] = col[1];
  col4[2] = col[2];
  col4[3] = 1.0f;
  const bool value_changed = ColorEdit4(label, col4, false);
  col[0] = col4[0];
  col[1] = col4[1];
  col[2] = col4[2];
  return value_changed;
>>>>>>> master
}

// Edit colors components (each component in 0.0f..1.0f range
// Use CTRL-Click to input value and TAB to go to next item.
<<<<<<< HEAD
bool ImGui::ColorEdit4(const char* label, float col[4], bool alpha)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext&     g         = *GImGui;
    const ImGuiStyle& style     = g.Style;
    const ImGuiID     id        = window->GetID(label);
    const float       w_full    = CalcItemWidth();
    const float       square_sz = (g.FontSize + style.FramePadding.y * 2.0f);
=======
bool ImGui::ColorEdit4(const char *label, float col[4], bool alpha) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w_full = CalcItemWidth();
  const float square_sz = (g.FontSize + style.FramePadding.y * 2.0f);
>>>>>>> master

  ImGuiColorEditMode edit_mode = window->DC.ColorEditMode;
  if (edit_mode == ImGuiColorEditMode_UserSelect ||
      edit_mode == ImGuiColorEditMode_UserSelectShowButton)
    edit_mode = g.ColorEditModeStorage.GetInt(id, 0) % 3;

<<<<<<< HEAD
    float f[4] = {col[0], col[1], col[2], col[3]};
    if (edit_mode == ImGuiColorEditMode_HSV) ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

    int i[4] = {IM_F32_TO_INT8_UNBOUND(f[0]),
        IM_F32_TO_INT8_UNBOUND(f[1]),
        IM_F32_TO_INT8_UNBOUND(f[2]),
        IM_F32_TO_INT8_UNBOUND(f[3])};

    int  components    = alpha ? 4 : 3;
    bool value_changed = false;
=======
  float f[4] = {col[0], col[1], col[2], col[3]};
  if (edit_mode == ImGuiColorEditMode_HSV)
    ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

  int i[4] = {IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]),
              IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3])};

  int components = alpha ? 4 : 3;
  bool value_changed = false;
>>>>>>> master

  BeginGroup();
  PushID(label);

<<<<<<< HEAD
    const bool hsv = (edit_mode == 1);
    switch (edit_mode)
    {
        case ImGuiColorEditMode_RGB:
        case ImGuiColorEditMode_HSV:
        {
            // RGB/HSV 0..255 Sliders
            const float w_items_all = w_full - (square_sz + style.ItemInnerSpacing.x);
            const float w_item_one  = ImMax(
                1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
            const float w_item_last =
                ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            const bool  hide_prefix     = (w_item_one <= CalcTextSize("M:999").x);
            const char* ids[4]          = {"##X", "##Y", "##Z", "##W"};
            const char* fmt_table[3][4] = {{"%3.0f", "%3.0f", "%3.0f", "%3.0f"},
                {"R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f"},
                {"H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f"}};
            const char** fmt = hide_prefix ? fmt_table[0] : hsv ? fmt_table[2] : fmt_table[1];

            PushItemWidth(w_item_one);
            for (int n = 0; n < components; n++) {
                if (n > 0) SameLine(0, style.ItemInnerSpacing.x);
                if (n + 1 == components) PushItemWidth(w_item_last);
                value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
            }
            PopItemWidth();
            PopItemWidth();
        }
        break;
        case ImGuiColorEditMode_HEX:
        {
            // RGB Hexadecimal Input
            const float w_slider_all = w_full - square_sz;
            char        buf[64];
            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
            PushItemWidth(w_slider_all - style.ItemInnerSpacing.x);
            if (InputText("##Text",
                    buf,
                    IM_ARRAYSIZE(buf),
                    ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed |= true;
                char* p = buf;
                while (*p == '#' || ImCharIsSpace(*p)) p++;
                i[0] = i[1] = i[2] = i[3] = 0;
                if (alpha)
                    sscanf(p,
                        "%02X%02X%02X%02X",
                        (unsigned int*)&i[0],
                        (unsigned int*)&i[1],
                        (unsigned int*)&i[2],
                        (unsigned int*)&i[3]);  // Treat at unsigned (%X is unsigned)
                else
                    sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
            }
            PopItemWidth();
        }
        break;
    }

    SameLine(0, style.ItemInnerSpacing.x);

    const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
    if (ColorButton(col_display))
        g.ColorEditModeStorage.SetInt(id, (edit_mode + 1) % 3);  // Don't set local copy of 'edit_mode' right away!

    // Recreate our own tooltip over's ColorButton() one because we want to
    // display correct alpha here
    if (IsItemHovered())
        SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X",
            col[0],
            col[1],
            col[2],
            col[3],
            IM_F32_TO_INT8_SAT(col[0]),
            IM_F32_TO_INT8_SAT(col[1]),
            IM_F32_TO_INT8_SAT(col[2]),
            IM_F32_TO_INT8_SAT(col[3]));

    if (window->DC.ColorEditMode == ImGuiColorEditMode_UserSelectShowButton) {
        SameLine(0, style.ItemInnerSpacing.x);
        const char* button_titles[3] = {"RGB", "HSV", "HEX"};
        if (ButtonEx(button_titles[edit_mode], ImVec2(0, 0), ImGuiButtonFlags_DontClosePopups))
            g.ColorEditModeStorage.SetInt(id, (edit_mode + 1) % 3);  // Don't set local copy of 'edit_mode' right away!
    }

    const char* label_display_end = FindRenderedTextEnd(label);
    if (label != label_display_end) {
        SameLine(0,
            (window->DC.ColorEditMode == ImGuiColorEditMode_UserSelectShowButton) ? -1.0f : style.ItemInnerSpacing.x);
        TextUnformatted(label, label_display_end);
=======
  const bool hsv = (edit_mode == 1);
  switch (edit_mode) {
  case ImGuiColorEditMode_RGB:
  case ImGuiColorEditMode_HSV: {
    // RGB/HSV 0..255 Sliders
    const float w_items_all = w_full - (square_sz + style.ItemInnerSpacing.x);
    const float w_item_one = ImMax(
        1.0f, (float)(int)((w_items_all -
                            (style.ItemInnerSpacing.x) * (components - 1)) /
                           (float)components));
    const float w_item_last =
        ImMax(1.0f, (float)(int)(w_items_all -
                                 (w_item_one + style.ItemInnerSpacing.x) *
                                     (components - 1)));

    const bool hide_prefix = (w_item_one <= CalcTextSize("M:999").x);
    const char *ids[4] = {"##X", "##Y", "##Z", "##W"};
    const char *fmt_table[3]
                         [4] = {{"%3.0f", "%3.0f", "%3.0f", "%3.0f"},
                                {"R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f"},
                                {"H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f"}};
    const char **fmt =
        hide_prefix ? fmt_table[0] : hsv ? fmt_table[2] : fmt_table[1];

    PushItemWidth(w_item_one);
    for (int n = 0; n < components; n++) {
      if (n > 0)
        SameLine(0, style.ItemInnerSpacing.x);
      if (n + 1 == components)
        PushItemWidth(w_item_last);
      value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
    }
    PopItemWidth();
    PopItemWidth();
  } break;
  case ImGuiColorEditMode_HEX: {
    // RGB Hexadecimal Input
    const float w_slider_all = w_full - square_sz;
    char buf[64];
    if (alpha)
      ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1],
                     i[2], i[3]);
    else
      ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
    PushItemWidth(w_slider_all - style.ItemInnerSpacing.x);
    if (InputText("##Text", buf, IM_ARRAYSIZE(buf),
                  ImGuiInputTextFlags_CharsHexadecimal |
                      ImGuiInputTextFlags_CharsUppercase)) {
      value_changed |= true;
      char *p = buf;
      while (*p == '#' || ImCharIsSpace(*p))
        p++;
      i[0] = i[1] = i[2] = i[3] = 0;
      if (alpha)
        sscanf(p, "%02X%02X%02X%02X", (unsigned int *)&i[0],
               (unsigned int *)&i[1], (unsigned int *)&i[2],
               (unsigned int *)&i[3]); // Treat at unsigned (%X is unsigned)
      else
        sscanf(p, "%02X%02X%02X", (unsigned int *)&i[0], (unsigned int *)&i[1],
               (unsigned int *)&i[2]);
>>>>>>> master
    }
    PopItemWidth();
  } break;
  }

<<<<<<< HEAD
    // Convert back
    for (int n = 0; n < 4; n++) f[n] = i[n] / 255.0f;
    if (edit_mode == 1) ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);

    if (value_changed) {
        col[0]            = f[0];
        col[1]            = f[1];
        col[2]            = f[2];
        if (alpha) col[3] = f[3];
    }
=======
  SameLine(0, style.ItemInnerSpacing.x);

  const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
  if (ColorButton(col_display))
    g.ColorEditModeStorage.SetInt(
        id,
        (edit_mode + 1) % 3); // Don't set local copy of 'edit_mode' right away!
>>>>>>> master

  // Recreate our own tooltip over's ColorButton() one because we want to
  // display correct alpha here
  if (IsItemHovered())
    SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0],
               col[1], col[2], col[3], IM_F32_TO_INT8_SAT(col[0]),
               IM_F32_TO_INT8_SAT(col[1]), IM_F32_TO_INT8_SAT(col[2]),
               IM_F32_TO_INT8_SAT(col[3]));

  if (window->DC.ColorEditMode == ImGuiColorEditMode_UserSelectShowButton) {
    SameLine(0, style.ItemInnerSpacing.x);
    const char *button_titles[3] = {"RGB", "HSV", "HEX"};
    if (ButtonEx(button_titles[edit_mode], ImVec2(0, 0),
                 ImGuiButtonFlags_DontClosePopups))
      g.ColorEditModeStorage.SetInt(
          id, (edit_mode + 1) %
                  3); // Don't set local copy of 'edit_mode' right away!
  }

  const char *label_display_end = FindRenderedTextEnd(label);
  if (label != label_display_end) {
    SameLine(
        0, (window->DC.ColorEditMode == ImGuiColorEditMode_UserSelectShowButton)
               ? -1.0f
               : style.ItemInnerSpacing.x);
    TextUnformatted(label, label_display_end);
  }

  // Convert back
  for (int n = 0; n < 4; n++)
    f[n] = i[n] / 255.0f;
  if (edit_mode == 1)
    ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);

  if (value_changed) {
    col[0] = f[0];
    col[1] = f[1];
    col[2] = f[2];
    if (alpha)
      col[3] = f[3];
  }

  PopID();
  EndGroup();

  return value_changed;
}

<<<<<<< HEAD
void ImGui::ColorEditMode(ImGuiColorEditMode mode)
{
    ImGuiWindow* window      = GetCurrentWindow();
    window->DC.ColorEditMode = mode;
}

// Horizontal separating line.
void ImGui::Separator()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    if (window->DC.ColumnsCount > 1) PopClipRect();

    float x1 = window->Pos.x;
    float x2 = window->Pos.x + window->Size.x;
    if (!window->DC.GroupStack.empty()) x1 += window->DC.IndentX;

    const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + 1.0f));
    ItemSize(ImVec2(0.0f, 0.0f));  // NB: we don't provide our width so that it
                                   // doesn't get feed back into AutoFit, we don't
                                   // provide height to not alter layout.
    if (!ItemAdd(bb, NULL)) {
        if (window->DC.ColumnsCount > 1) PushColumnClipRect();
        return;
    }

    window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Border));

    ImGuiContext& g = *GImGui;
    if (g.LogEnabled) LogText(IM_NEWLINE "--------------------------------");

    if (window->DC.ColumnsCount > 1) {
        PushColumnClipRect();
        window->DC.ColumnsCellMinY = window->DC.CursorPos.y;
    }
}

void ImGui::Spacing()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;
    ItemSize(ImVec2(0, 0));
}

void ImGui::Dummy(const ImVec2& size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;
=======
void ImGui::ColorEditMode(ImGuiColorEditMode mode) {
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.ColorEditMode = mode;
}

// Horizontal separating line.
void ImGui::Separator() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  if (window->DC.ColumnsCount > 1)
    PopClipRect();

  float x1 = window->Pos.x;
  float x2 = window->Pos.x + window->Size.x;
  if (!window->DC.GroupStack.empty())
    x1 += window->DC.IndentX;

  const ImRect bb(ImVec2(x1, window->DC.CursorPos.y),
                  ImVec2(x2, window->DC.CursorPos.y + 1.0f));
  ItemSize(ImVec2(0.0f, 0.0f)); // NB: we don't provide our width so that it
                                // doesn't get feed back into AutoFit, we don't
                                // provide height to not alter layout.
  if (!ItemAdd(bb, NULL)) {
    if (window->DC.ColumnsCount > 1)
      PushColumnClipRect();
    return;
  }

  window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y),
                            GetColorU32(ImGuiCol_Border));

  ImGuiContext &g = *GImGui;
  if (g.LogEnabled)
    LogText(IM_NEWLINE "--------------------------------");

  if (window->DC.ColumnsCount > 1) {
    PushColumnClipRect();
    window->DC.ColumnsCellMinY = window->DC.CursorPos.y;
  }
}

void ImGui::Spacing() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;
  ItemSize(ImVec2(0, 0));
}

void ImGui::Dummy(const ImVec2 &size) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;
>>>>>>> master

  const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
  ItemSize(bb);
  ItemAdd(bb, NULL);
}

bool ImGui::IsRectVisible(const ImVec2 &size) {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->ClipRect.Overlaps(
      ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2 &rect_min, const ImVec2 &rect_max) {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}

// Lock horizontal starting position + capture group bounding box into one
// "item" (so you can use IsItemHovered() or layout primitives such as
// SameLine() on whole group, etc.)
<<<<<<< HEAD
void ImGui::BeginGroup()
{
    ImGuiWindow* window = GetCurrentWindow();

    window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
    ImGuiGroupData& group_data                 = window->DC.GroupStack.back();
    group_data.BackupCursorPos                 = window->DC.CursorPos;
    group_data.BackupCursorMaxPos              = window->DC.CursorMaxPos;
    group_data.BackupIndentX                   = window->DC.IndentX;
    group_data.BackupGroupOffsetX              = window->DC.GroupOffsetX;
    group_data.BackupCurrentLineHeight         = window->DC.CurrentLineHeight;
    group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
    group_data.BackupLogLinePosY               = window->DC.LogLinePosY;
    group_data.BackupActiveIdIsAlive           = GImGui->ActiveIdIsAlive;
    group_data.AdvanceCursor                   = true;

    window->DC.GroupOffsetX      = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
    window->DC.IndentX           = window->DC.GroupOffsetX;
    window->DC.CursorMaxPos      = window->DC.CursorPos;
    window->DC.CurrentLineHeight = 0.0f;
    window->DC.LogLinePosY       = window->DC.CursorPos.y - 9999.0f;
}

void ImGui::EndGroup()
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();

    IM_ASSERT(!window->DC.GroupStack.empty());  // Mismatched BeginGroup()/EndGroup() calls
=======
void ImGui::BeginGroup() {
  ImGuiWindow *window = GetCurrentWindow();

  window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
  ImGuiGroupData &group_data = window->DC.GroupStack.back();
  group_data.BackupCursorPos = window->DC.CursorPos;
  group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
  group_data.BackupIndentX = window->DC.IndentX;
  group_data.BackupGroupOffsetX = window->DC.GroupOffsetX;
  group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
  group_data.BackupCurrentLineTextBaseOffset =
      window->DC.CurrentLineTextBaseOffset;
  group_data.BackupLogLinePosY = window->DC.LogLinePosY;
  group_data.BackupActiveIdIsAlive = GImGui->ActiveIdIsAlive;
  group_data.AdvanceCursor = true;

  window->DC.GroupOffsetX =
      window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
  window->DC.IndentX = window->DC.GroupOffsetX;
  window->DC.CursorMaxPos = window->DC.CursorPos;
  window->DC.CurrentLineHeight = 0.0f;
  window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
}

void ImGui::EndGroup() {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();

  IM_ASSERT(!window->DC.GroupStack
                 .empty()); // Mismatched BeginGroup()/EndGroup() calls
>>>>>>> master

  ImGuiGroupData &group_data = window->DC.GroupStack.back();

<<<<<<< HEAD
    ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
    group_bb.Max.y -= g.Style.ItemSpacing.y;  // Cancel out last vertical spacing
                                              // because we are adding one
                                              // ourselves.
    group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

    window->DC.CursorPos                 = group_data.BackupCursorPos;
    window->DC.CursorMaxPos              = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.CurrentLineHeight         = group_data.BackupCurrentLineHeight;
    window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
    window->DC.IndentX                   = group_data.BackupIndentX;
    window->DC.GroupOffsetX              = group_data.BackupGroupOffsetX;
    window->DC.LogLinePosY               = window->DC.CursorPos.y - 9999.0f;

    if (group_data.AdvanceCursor) {
        window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset,
            group_data.BackupCurrentLineTextBaseOffset);  // FIXME: Incorrect,
                                                          // we should grab the
                                                          // base offset from
                                                          // the *first line*
                                                          // of the group but
                                                          // it is hard to
                                                          // obtain now.
        ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
        ItemAdd(group_bb, NULL);
    }

    // If the current ActiveId was declared within the boundary of our group, we
    // copy it to LastItemId so IsItemActive() will function on the entire group.
    // It would be be neater if we replaced window.DC.LastItemId by e.g. 'bool
    // LastItemIsActive', but if you search for LastItemId you'll notice it is
    // only used in that context.
    const bool active_id_within_group = (!group_data.BackupActiveIdIsAlive && g.ActiveIdIsAlive && g.ActiveId &&
                                         g.ActiveIdWindow->RootWindow == window->RootWindow);
    if (active_id_within_group) window->DC.LastItemId = g.ActiveId;
    if (active_id_within_group && g.HoveredId == g.ActiveId)
        window->DC.LastItemHoveredAndUsable = window->DC.LastItemHoveredRect = true;
=======
  ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
  group_bb.Max.y -= g.Style.ItemSpacing.y; // Cancel out last vertical spacing
                                           // because we are adding one
                                           // ourselves.
  group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

  window->DC.CursorPos = group_data.BackupCursorPos;
  window->DC.CursorMaxPos =
      ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
  window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
  window->DC.CurrentLineTextBaseOffset =
      group_data.BackupCurrentLineTextBaseOffset;
  window->DC.IndentX = group_data.BackupIndentX;
  window->DC.GroupOffsetX = group_data.BackupGroupOffsetX;
  window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;

  if (group_data.AdvanceCursor) {
    window->DC.CurrentLineTextBaseOffset =
        ImMax(window->DC.PrevLineTextBaseOffset,
              group_data.BackupCurrentLineTextBaseOffset); // FIXME: Incorrect,
                                                           // we should grab the
                                                           // base offset from
                                                           // the *first line*
                                                           // of the group but
                                                           // it is hard to
                                                           // obtain now.
    ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
    ItemAdd(group_bb, NULL);
  }

  // If the current ActiveId was declared within the boundary of our group, we
  // copy it to LastItemId so IsItemActive() will function on the entire group.
  // It would be be neater if we replaced window.DC.LastItemId by e.g. 'bool
  // LastItemIsActive', but if you search for LastItemId you'll notice it is
  // only used in that context.
  const bool active_id_within_group =
      (!group_data.BackupActiveIdIsAlive && g.ActiveIdIsAlive && g.ActiveId &&
       g.ActiveIdWindow->RootWindow == window->RootWindow);
  if (active_id_within_group)
    window->DC.LastItemId = g.ActiveId;
  if (active_id_within_group && g.HoveredId == g.ActiveId)
    window->DC.LastItemHoveredAndUsable = window->DC.LastItemHoveredRect = true;
>>>>>>> master

  window->DC.GroupStack.pop_back();

<<<<<<< HEAD
    // window->DrawList->AddRect(group_bb.Min, group_bb.Max,
    // IM_COL32(255,0,255,255));   // Debug
=======
  // window->DrawList->AddRect(group_bb.Min, group_bb.Max,
  // IM_COL32(255,0,255,255));   // Debug
>>>>>>> master
}

// Gets back to previous line and continue with horizontal layout
//      pos_x == 0      : follow right after previous item
//      pos_x != 0      : align to specified x position (relative to
//      window/group left)
//      spacing_w < 0   : use default spacing if pos_x == 0, no spacing if pos_x
//      != 0
//      spacing_w >= 0  : enforce spacing amount
<<<<<<< HEAD
void ImGui::SameLine(float pos_x, float spacing_w)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    if (pos_x != 0.0f) {
        if (spacing_w < 0.0f) spacing_w = 0.0f;
        window->DC.CursorPos.x =
            window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffsetX + window->DC.ColumnsOffsetX;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    else
    {
        if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
        window->DC.CursorPos.x          = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y          = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrentLineHeight         = window->DC.PrevLineHeight;
    window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;
    if (window->DC.CurrentLineHeight > 0.0f)  // In the event that we are on a line
                                              // with items that is smaller that
                                              // FontSize high, we will preserve
                                              // its height.
        ItemSize(ImVec2(0, 0));
    else
        ItemSize(ImVec2(0.0f, GImGui->FontSize));
}

void ImGui::NextColumn()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems || window->DC.ColumnsCount <= 1) return;
=======
void ImGui::SameLine(float pos_x, float spacing_w) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  if (pos_x != 0.0f) {
    if (spacing_w < 0.0f)
      spacing_w = 0.0f;
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x +
                             spacing_w + window->DC.GroupOffsetX +
                             window->DC.ColumnsOffsetX;
    window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
  } else {
    if (spacing_w < 0.0f)
      spacing_w = g.Style.ItemSpacing.x;
    window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
    window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
  }
  window->DC.CurrentLineHeight = window->DC.PrevLineHeight;
  window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems)
    return;
  if (window->DC.CurrentLineHeight > 0.0f) // In the event that we are on a line
                                           // with items that is smaller that
                                           // FontSize high, we will preserve
                                           // its height.
    ItemSize(ImVec2(0, 0));
  else
    ItemSize(ImVec2(0.0f, GImGui->FontSize));
}

void ImGui::NextColumn() {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems || window->DC.ColumnsCount <= 1)
    return;
>>>>>>> master

  ImGuiContext &g = *GImGui;
  PopItemWidth();
  PopClipRect();

  window->DC.ColumnsCellMaxY =
      ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
  if (++window->DC.ColumnsCurrent < window->DC.ColumnsCount) {
    // Columns 1+ cancel out IndentX
    window->DC.ColumnsOffsetX = GetColumnOffset(window->DC.ColumnsCurrent) -
                                window->DC.IndentX + g.Style.ItemSpacing.x;
    window->DrawList->ChannelsSetCurrent(window->DC.ColumnsCurrent);
  } else {
    window->DC.ColumnsCurrent = 0;
    window->DC.ColumnsOffsetX = 0.0f;
    window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY;
    window->DrawList->ChannelsSetCurrent(0);
  }
  window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX +
                                        window->DC.ColumnsOffsetX);
  window->DC.CursorPos.y = window->DC.ColumnsCellMinY;
  window->DC.CurrentLineHeight = 0.0f;
  window->DC.CurrentLineTextBaseOffset = 0.0f;

  PushColumnClipRect();
  PushItemWidth(GetColumnWidth() * 0.65f); // FIXME: Move on columns setup
}

int ImGui::GetColumnIndex() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.ColumnsCurrent;
}

int ImGui::GetColumnsCount() {
  ImGuiWindow *window = GetCurrentWindowRead();
  return window->DC.ColumnsCount;
}

static float GetDraggedColumnOffset(int column_index) {
  // Active (dragged) column always follow mouse. The reason we need this is
  // that dragging a column to the right edge of an auto-resizing
  // window creates a feedback loop because we store normalized positions. So
  // while dragging we enforce absolute positioning.
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = ImGui::GetCurrentWindowRead();
  IM_ASSERT(column_index > 0); // We cannot drag column 0. If you get this
                               // assert you may have a conflict between the ID
                               // of your columns and another widgets.
  IM_ASSERT(g.ActiveId == window->DC.ColumnsSetId + ImGuiID(column_index));

  float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x - window->Pos.x;
  x = ImClamp(
      x, ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing,
      ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

  return (float)(int)x;
}

float ImGui::GetColumnOffset(int column_index) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindowRead();
  if (column_index < 0)
    column_index = window->DC.ColumnsCurrent;

  if (g.ActiveId) {
    const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
    if (g.ActiveId == column_id)
      return GetDraggedColumnOffset(column_index);
  }

  IM_ASSERT(column_index < window->DC.ColumnsData.Size);
  const float t = window->DC.ColumnsData[column_index].OffsetNorm;
  const float x_offset = window->DC.ColumnsMinX +
                         t * (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
  return (float)(int)x_offset;
}

void ImGui::SetColumnOffset(int column_index, float offset) {
  ImGuiWindow *window = GetCurrentWindow();
  if (column_index < 0)
    column_index = window->DC.ColumnsCurrent;

  IM_ASSERT(column_index < window->DC.ColumnsData.Size);
  const float t = (offset - window->DC.ColumnsMinX) /
                  (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
  window->DC.ColumnsData[column_index].OffsetNorm = t;

  const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
  window->DC.StateStorage->SetFloat(column_id, t);
}

float ImGui::GetColumnWidth(int column_index) {
  ImGuiWindow *window = GetCurrentWindowRead();
  if (column_index < 0)
    column_index = window->DC.ColumnsCurrent;

  float w = GetColumnOffset(column_index + 1) - GetColumnOffset(column_index);
  return w;
}

static void PushColumnClipRect(int column_index) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  if (column_index < 0)
    column_index = window->DC.ColumnsCurrent;

  float x1 = ImFloor(0.5f + window->Pos.x +
                     ImGui::GetColumnOffset(column_index) - 1.0f);
  float x2 = ImFloor(0.5f + window->Pos.x +
                     ImGui::GetColumnOffset(column_index + 1) - 1.0f);
  ImGui::PushClipRect(ImVec2(x1, -FLT_MAX), ImVec2(x2, +FLT_MAX), true);
}

void ImGui::Columns(int columns_count, const char *id, bool border) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();
  IM_ASSERT(columns_count >= 1);

  if (window->DC.ColumnsCount != 1) {
    if (window->DC.ColumnsCurrent != 0)
      ItemSize(ImVec2(0, 0)); // Advance to column 0
    PopItemWidth();
    PopClipRect();
    window->DrawList->ChannelsMerge();

<<<<<<< HEAD
    window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
    if (++window->DC.ColumnsCurrent < window->DC.ColumnsCount) {
        // Columns 1+ cancel out IndentX
        window->DC.ColumnsOffsetX =
            GetColumnOffset(window->DC.ColumnsCurrent) - window->DC.IndentX + g.Style.ItemSpacing.x;
        window->DrawList->ChannelsSetCurrent(window->DC.ColumnsCurrent);
    }
    else
    {
        window->DC.ColumnsCurrent  = 0;
        window->DC.ColumnsOffsetX  = 0.0f;
        window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY;
        window->DrawList->ChannelsSetCurrent(0);
    }
    window->DC.CursorPos.x               = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
    window->DC.CursorPos.y               = window->DC.ColumnsCellMinY;
    window->DC.CurrentLineHeight         = 0.0f;
    window->DC.CurrentLineTextBaseOffset = 0.0f;
=======
    window->DC.ColumnsCellMaxY =
        ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
    window->DC.CursorPos.y = window->DC.ColumnsCellMaxY;
  }
>>>>>>> master

  // Draw columns borders and handle resize at the time of "closing" a columns
  // set
  if (window->DC.ColumnsCount != columns_count &&
      window->DC.ColumnsCount != 1 && window->DC.ColumnsShowBorders &&
      !window->SkipItems) {
    const float y1 = window->DC.ColumnsStartPosY;
    const float y2 = window->DC.CursorPos.y;
    for (int i = 1; i < window->DC.ColumnsCount; i++) {
      float x = window->Pos.x + GetColumnOffset(i);
      const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(i);
      const ImRect column_rect(ImVec2(x - 4, y1), ImVec2(x + 4, y2));
      if (IsClippedEx(column_rect, &column_id, false))
        continue;

      bool hovered, held;
      ButtonBehavior(column_rect, column_id, &hovered, &held);
      if (hovered || held)
        g.MouseCursor = ImGuiMouseCursor_ResizeEW;

      // Draw before resize so our items positioning are in sync with the line
      // being drawn
      const ImU32 col = GetColorU32(held ? ImGuiCol_ColumnActive
                                         : hovered ? ImGuiCol_ColumnHovered
                                                   : ImGuiCol_Column);
      const float xi = (float)(int)x;
      window->DrawList->AddLine(ImVec2(xi, y1 + 1.0f), ImVec2(xi, y2), col);

      if (held) {
        if (g.ActiveIdIsJustActivated)
          g.ActiveIdClickOffset.x -= 4; // Store from center of column line (we
                                        // used a 8 wide rect for columns
                                        // clicking)
        x = GetDraggedColumnOffset(i);
        SetColumnOffset(i, x);
      }
    }
  }

  // Differentiate column ID with an arbitrary prefix for cases where users name
  // their columns set the same as another widget.
  // In addition, when an identifier isn't explicitly provided we include the
  // number of columns in the hash to make it uniquer.
  PushID(0x11223347 + (id ? 0 : columns_count));
  window->DC.ColumnsSetId = window->GetID(id ? id : "columns");
  PopID();

  // Set state for first column
  window->DC.ColumnsCurrent = 0;
  window->DC.ColumnsCount = columns_count;
  window->DC.ColumnsShowBorders = border;

  const float content_region_width = (window->SizeContentsExplicit.x != 0.0f)
                                         ? window->SizeContentsExplicit.x
                                         : window->Size.x;
  window->DC.ColumnsMinX = window->DC.IndentX; // Lock our horizontal range
  window->DC.ColumnsMaxX =
      content_region_width - window->Scroll.x -
      ((window->Flags & ImGuiWindowFlags_NoScrollbar)
           ? 0
           : g.Style.ScrollbarSize); // - window->WindowPadding().x;
  window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
  window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY =
      window->DC.CursorPos.y;
  window->DC.ColumnsOffsetX = 0.0f;
  window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX +
                                        window->DC.ColumnsOffsetX);

  if (window->DC.ColumnsCount != 1) {
    // Cache column offsets
    window->DC.ColumnsData.resize(columns_count + 1);
    for (int column_index = 0; column_index < columns_count + 1;
         column_index++) {
      const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
      KeepAliveID(column_id);
      const float default_t = column_index / (float)window->DC.ColumnsCount;
      const float t = window->DC.StateStorage->GetFloat(
          column_id, default_t); // Cheaply store our floating point value
      // inside the integer (could store a union into
      // the map?)
      window->DC.ColumnsData[column_index].OffsetNorm = t;
    }
    window->DrawList->ChannelsSplit(window->DC.ColumnsCount);
    PushColumnClipRect();
    PushItemWidth(GetColumnWidth() * 0.65f);
  } else {
    window->DC.ColumnsData.resize(0);
  }
}

void ImGui::Indent(float indent_w) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.IndentX += (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
  window->DC.CursorPos.x =
      window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::Unindent(float indent_w) {
  ImGuiContext &g = *GImGui;
  ImGuiWindow *window = GetCurrentWindow();
  window->DC.IndentX -= (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
  window->DC.CursorPos.x =
      window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

<<<<<<< HEAD
static float GetDraggedColumnOffset(int column_index)
{
    // Active (dragged) column always follow mouse. The reason we need this is
    // that dragging a column to the right edge of an auto-resizing
    // window creates a feedback loop because we store normalized positions. So
    // while dragging we enforce absolute positioning.
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = ImGui::GetCurrentWindowRead();
    IM_ASSERT(column_index > 0);  // We cannot drag column 0. If you get this
                                  // assert you may have a conflict between the ID
                                  // of your columns and another widgets.
    IM_ASSERT(g.ActiveId == window->DC.ColumnsSetId + ImGuiID(column_index));

    float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x - window->Pos.x;
    x       = ImClamp(x,
        ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing,
        ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

    return (float)(int)x;
}

float ImGui::GetColumnOffset(int column_index)
{
    ImGuiContext& g                    = *GImGui;
    ImGuiWindow*  window               = GetCurrentWindowRead();
    if (column_index < 0) column_index = window->DC.ColumnsCurrent;

    if (g.ActiveId) {
        const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
        if (g.ActiveId == column_id) return GetDraggedColumnOffset(column_index);
    }

    IM_ASSERT(column_index < window->DC.ColumnsData.Size);
    const float t        = window->DC.ColumnsData[column_index].OffsetNorm;
    const float x_offset = window->DC.ColumnsMinX + t * (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
    return (float)(int)x_offset;
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
    ImGuiWindow* window                = GetCurrentWindow();
    if (column_index < 0) column_index = window->DC.ColumnsCurrent;

    IM_ASSERT(column_index < window->DC.ColumnsData.Size);
    const float t = (offset - window->DC.ColumnsMinX) / (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
    window->DC.ColumnsData[column_index].OffsetNorm = t;

    const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
    window->DC.StateStorage->SetFloat(column_id, t);
}

float ImGui::GetColumnWidth(int column_index)
{
    ImGuiWindow* window                = GetCurrentWindowRead();
    if (column_index < 0) column_index = window->DC.ColumnsCurrent;

    float w = GetColumnOffset(column_index + 1) - GetColumnOffset(column_index);
    return w;
}

static void PushColumnClipRect(int column_index)
{
    ImGuiWindow* window                = ImGui::GetCurrentWindow();
    if (column_index < 0) column_index = window->DC.ColumnsCurrent;

    float x1 = ImFloor(0.5f + window->Pos.x + ImGui::GetColumnOffset(column_index) - 1.0f);
    float x2 = ImFloor(0.5f + window->Pos.x + ImGui::GetColumnOffset(column_index + 1) - 1.0f);
    ImGui::PushClipRect(ImVec2(x1, -FLT_MAX), ImVec2(x2, +FLT_MAX), true);
}

void ImGui::Columns(int columns_count, const char* id, bool border)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();
    IM_ASSERT(columns_count >= 1);

    if (window->DC.ColumnsCount != 1) {
        if (window->DC.ColumnsCurrent != 0) ItemSize(ImVec2(0, 0));  // Advance to column 0
        PopItemWidth();
        PopClipRect();
        window->DrawList->ChannelsMerge();

        window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
        window->DC.CursorPos.y     = window->DC.ColumnsCellMaxY;
    }

    // Draw columns borders and handle resize at the time of "closing" a columns
    // set
    if (window->DC.ColumnsCount != columns_count && window->DC.ColumnsCount != 1 && window->DC.ColumnsShowBorders &&
        !window->SkipItems)
    {
        const float y1 = window->DC.ColumnsStartPosY;
        const float y2 = window->DC.CursorPos.y;
        for (int i = 1; i < window->DC.ColumnsCount; i++) {
            float         x         = window->Pos.x + GetColumnOffset(i);
            const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(i);
            const ImRect  column_rect(ImVec2(x - 4, y1), ImVec2(x + 4, y2));
            if (IsClippedEx(column_rect, &column_id, false)) continue;

            bool hovered, held;
            ButtonBehavior(column_rect, column_id, &hovered, &held);
            if (hovered || held) g.MouseCursor = ImGuiMouseCursor_ResizeEW;

            // Draw before resize so our items positioning are in sync with the line
            // being drawn
            const ImU32 col =
                GetColorU32(held ? ImGuiCol_ColumnActive : hovered ? ImGuiCol_ColumnHovered : ImGuiCol_Column);
            const float xi = (float)(int)x;
            window->DrawList->AddLine(ImVec2(xi, y1 + 1.0f), ImVec2(xi, y2), col);

            if (held) {
                if (g.ActiveIdIsJustActivated) g.ActiveIdClickOffset.x -= 4;  // Store from center of column line (we
                                                                              // used a 8 wide rect for columns
                                                                              // clicking)
                x = GetDraggedColumnOffset(i);
                SetColumnOffset(i, x);
            }
        }
    }

    // Differentiate column ID with an arbitrary prefix for cases where users name
    // their columns set the same as another widget.
    // In addition, when an identifier isn't explicitly provided we include the
    // number of columns in the hash to make it uniquer.
    PushID(0x11223347 + (id ? 0 : columns_count));
    window->DC.ColumnsSetId = window->GetID(id ? id : "columns");
    PopID();

    // Set state for first column
    window->DC.ColumnsCurrent     = 0;
    window->DC.ColumnsCount       = columns_count;
    window->DC.ColumnsShowBorders = border;

    const float content_region_width =
        (window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : window->Size.x;
    window->DC.ColumnsMinX = window->DC.IndentX;  // Lock our horizontal range
    window->DC.ColumnsMaxX =
        content_region_width - window->Scroll.x -
        ((window->Flags & ImGuiWindowFlags_NoScrollbar) ? 0 : g.Style.ScrollbarSize);  // - window->WindowPadding().x;
    window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
    window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.CursorPos.y;
    window->DC.ColumnsOffsetX                               = 0.0f;
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);

    if (window->DC.ColumnsCount != 1) {
        // Cache column offsets
        window->DC.ColumnsData.resize(columns_count + 1);
        for (int column_index = 0; column_index < columns_count + 1; column_index++) {
            const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
            KeepAliveID(column_id);
            const float default_t = column_index / (float)window->DC.ColumnsCount;
            const float t =
                window->DC.StateStorage->GetFloat(column_id, default_t);  // Cheaply store our floating point value
            // inside the integer (could store a union into
            // the map?)
            window->DC.ColumnsData[column_index].OffsetNorm = t;
        }
        window->DrawList->ChannelsSplit(window->DC.ColumnsCount);
        PushColumnClipRect();
        PushItemWidth(GetColumnWidth() * 0.65f);
    }
    else
    {
        window->DC.ColumnsData.resize(0);
    }
}

void ImGui::Indent(float indent_w)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();
    window->DC.IndentX += (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::Unindent(float indent_w)
{
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = GetCurrentWindow();
    window->DC.IndentX -= (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::TreePush(const char* str_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(str_id ? str_id : "#TreePush");
}

void ImGui::TreePush(const void* ptr_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(ptr_id ? ptr_id : (const void*)"#TreePush");
}

void ImGui::TreePushRawID(ImGuiID id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    window->IDStack.push_back(id);
}

void ImGui::TreePop()
{
    ImGuiWindow* window = GetCurrentWindow();
    Unindent();
    window->DC.TreeDepth--;
    PopID();
}

void ImGui::Value(const char* prefix, bool b) { Text("%s: %s", prefix, (b ? "true" : "false")); }

void ImGui::Value(const char* prefix, int v) { Text("%s: %d", prefix, v); }

void ImGui::Value(const char* prefix, unsigned int v) { Text("%s: %d", prefix, v); }

void ImGui::Value(const char* prefix, float v, const char* float_format)
{
    if (float_format) {
        char fmt[64];
        ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
        Text(fmt, prefix, v);
    }
    else
    {
        Text("%s: %.3f", prefix, v);
    }
=======
void ImGui::TreePush(const char *str_id) {
  ImGuiWindow *window = GetCurrentWindow();
  Indent();
  window->DC.TreeDepth++;
  PushID(str_id ? str_id : "#TreePush");
}

void ImGui::TreePush(const void *ptr_id) {
  ImGuiWindow *window = GetCurrentWindow();
  Indent();
  window->DC.TreeDepth++;
  PushID(ptr_id ? ptr_id : (const void *)"#TreePush");
}

void ImGui::TreePushRawID(ImGuiID id) {
  ImGuiWindow *window = GetCurrentWindow();
  Indent();
  window->DC.TreeDepth++;
  window->IDStack.push_back(id);
}

void ImGui::TreePop() {
  ImGuiWindow *window = GetCurrentWindow();
  Unindent();
  window->DC.TreeDepth--;
  PopID();
}

void ImGui::Value(const char *prefix, bool b) {
  Text("%s: %s", prefix, (b ? "true" : "false"));
}

void ImGui::Value(const char *prefix, int v) { Text("%s: %d", prefix, v); }

void ImGui::Value(const char *prefix, unsigned int v) {
  Text("%s: %d", prefix, v);
}

void ImGui::Value(const char *prefix, float v, const char *float_format) {
  if (float_format) {
    char fmt[64];
    ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
    Text(fmt, prefix, v);
  } else {
    Text("%s: %.3f", prefix, v);
  }
>>>>>>> master
}

// FIXME: May want to remove those helpers?
void ImGui::ValueColor(const char *prefix, const ImVec4 &v) {
  Text("%s: (%.2f,%.2f,%.2f,%.2f)", prefix, v.x, v.y, v.z, v.w);
  SameLine();
  ColorButton(v, true);
}

void ImGui::ValueColor(const char *prefix, ImU32 v) {
  Text("%s: %08X", prefix, v);
  SameLine();
  ColorButton(ColorConvertU32ToFloat4(v), true);
}

//-----------------------------------------------------------------------------
// PLATFORM DEPENDENT HELPERS
//-----------------------------------------------------------------------------

<<<<<<< HEAD
#if defined(_WIN32) && !defined(_WINDOWS_) && \
    (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS) || !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS))
=======
#if defined(_WIN32) && !defined(_WINDOWS_) &&                                  \
    (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS) ||                  \
     !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS))
>>>>>>> master
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Win32 API clipboard implementation
#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#endif

<<<<<<< HEAD
static const char* GetClipboardTextFn_DefaultImpl(void*)
{
    static ImVector<char> buf_local;
    buf_local.clear();
    if (!OpenClipboard(NULL)) return NULL;
    HANDLE wbuf_handle = GetClipboardData(CF_UNICODETEXT);
    if (wbuf_handle == NULL) return NULL;
    if (ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle)) {
        int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
        buf_local.resize(buf_len);
        ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
    }
    GlobalUnlock(wbuf_handle);
    CloseClipboard();
    return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    if (!OpenClipboard(NULL)) return;
    const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
    HGLOBAL   wbuf_handle = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
    if (wbuf_handle == NULL) return;
    ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle);
    ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
    GlobalUnlock(wbuf_handle);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, wbuf_handle);
    CloseClipboard();
=======
static const char *GetClipboardTextFn_DefaultImpl(void *) {
  static ImVector<char> buf_local;
  buf_local.clear();
  if (!OpenClipboard(NULL))
    return NULL;
  HANDLE wbuf_handle = GetClipboardData(CF_UNICODETEXT);
  if (wbuf_handle == NULL)
    return NULL;
  if (ImWchar *wbuf_global = (ImWchar *)GlobalLock(wbuf_handle)) {
    int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
    buf_local.resize(buf_len);
    ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
  }
  GlobalUnlock(wbuf_handle);
  CloseClipboard();
  return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(void *, const char *text) {
  if (!OpenClipboard(NULL))
    return;
  const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
  HGLOBAL wbuf_handle =
      GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
  if (wbuf_handle == NULL)
    return;
  ImWchar *wbuf_global = (ImWchar *)GlobalLock(wbuf_handle);
  ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
  GlobalUnlock(wbuf_handle);
  EmptyClipboard();
  SetClipboardData(CF_UNICODETEXT, wbuf_handle);
  CloseClipboard();
>>>>>>> master
}

#else

// Local ImGui-only clipboard implementation, if user hasn't defined better
// clipboard handlers
<<<<<<< HEAD
static const char* GetClipboardTextFn_DefaultImpl(void*) { return GImGui->PrivateClipboard; }

// Local ImGui-only clipboard implementation, if user hasn't defined better
// clipboard handlers
static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    ImGuiContext& g = *GImGui;
    if (g.PrivateClipboard) {
        ImGui::MemFree(g.PrivateClipboard);
        g.PrivateClipboard = NULL;
    }
    const char* text_end = text + strlen(text);
    g.PrivateClipboard   = (char*)ImGui::MemAlloc((size_t)(text_end - text) + 1);
    memcpy(g.PrivateClipboard, text, (size_t)(text_end - text));
    g.PrivateClipboard[(int)(text_end - text)] = 0;
=======
static const char *GetClipboardTextFn_DefaultImpl(void *) {
  return GImGui->PrivateClipboard;
}

// Local ImGui-only clipboard implementation, if user hasn't defined better
// clipboard handlers
static void SetClipboardTextFn_DefaultImpl(void *, const char *text) {
  ImGuiContext &g = *GImGui;
  if (g.PrivateClipboard) {
    ImGui::MemFree(g.PrivateClipboard);
    g.PrivateClipboard = NULL;
  }
  const char *text_end = text + strlen(text);
  g.PrivateClipboard = (char *)ImGui::MemAlloc((size_t)(text_end - text) + 1);
  memcpy(g.PrivateClipboard, text, (size_t)(text_end - text));
  g.PrivateClipboard[(int)(text_end - text)] = 0;
>>>>>>> master
}

#endif

// Win32 API IME support (for Asian languages, etc.)
#if defined(_WIN32) && !defined(__GNUC__) &&                                   \
    !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

<<<<<<< HEAD
static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y)
{
    // Notify OS Input Method Editor of text input position
    if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
        if (HIMC himc = ImmGetContext(hwnd)) {
            COMPOSITIONFORM cf;
            cf.ptCurrentPos.x = x;
            cf.ptCurrentPos.y = y;
            cf.dwStyle        = CFS_FORCE_POSITION;
            ImmSetCompositionWindow(himc, &cf);
        }
=======
static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y) {
  // Notify OS Input Method Editor of text input position
  if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
    if (HIMC himc = ImmGetContext(hwnd)) {
      COMPOSITIONFORM cf;
      cf.ptCurrentPos.x = x;
      cf.ptCurrentPos.y = y;
      cf.dwStyle = CFS_FORCE_POSITION;
      ImmSetCompositionWindow(himc, &cf);
    }
>>>>>>> master
}

#else

static void ImeSetInputScreenPosFn_DefaultImpl(int, int) {}

#endif

//-----------------------------------------------------------------------------
// HELP
//-----------------------------------------------------------------------------

<<<<<<< HEAD
void ImGui::ShowMetricsWindow(bool* p_open)
{
    if (ImGui::Begin("ImGui Metrics", p_open)) {
        ImGui::Text("ImGui %s", ImGui::GetVersion());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate);
        ImGui::Text("%d vertices, %d indices (%d triangles)",
            ImGui::GetIO().MetricsRenderVertices,
            ImGui::GetIO().MetricsRenderIndices,
            ImGui::GetIO().MetricsRenderIndices / 3);
        ImGui::Text("%d allocations", ImGui::GetIO().MetricsAllocs);
        static bool show_clip_rects = true;
        ImGui::Checkbox("Show clipping rectangles when hovering a ImDrawCmd", &show_clip_rects);
        ImGui::Separator();

        struct Funcs
        {
            static void NodeDrawList(ImDrawList* draw_list, const char* label)
            {
                bool node_open = ImGui::TreeNode(draw_list,
                    "%s: '%s' %d vtx, %d indices, %d cmds",
                    label,
                    draw_list->_OwnerName ? draw_list->_OwnerName : "",
                    draw_list->VtxBuffer.Size,
                    draw_list->IdxBuffer.Size,
                    draw_list->CmdBuffer.Size);
                if (draw_list == ImGui::GetWindowDrawList()) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImColor(255, 100, 100), "CURRENTLY APPENDING");  // Can't display stats for
                                                                                        // active draw list! (we
                                                                                        // don't have the data
                                                                                        // double-buffered)
                    if (node_open) ImGui::TreePop();
                    return;
                }
                if (!node_open) return;

                ImDrawList* overlay_draw_list = &GImGui->OverlayDrawList;  // Render additional visuals into the
                                                                           // top-most draw list
                overlay_draw_list->PushClipRectFullScreen();
                int elem_offset = 0;
                for (const ImDrawCmd *pcmd = draw_list->CmdBuffer.begin(); pcmd < draw_list->CmdBuffer.end();
                     elem_offset += pcmd->ElemCount, pcmd++)
                {
                    if (pcmd->UserCallback) {
                        ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
                        continue;
                    }
                    ImDrawIdx* idx_buffer     = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
                    bool       pcmd_node_open = ImGui::TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()),
                        "Draw %-4d %s vtx, tex = %p, clip_rect = "
                        "(%.0f,%.0f)..(%.0f,%.0f)",
                        pcmd->ElemCount,
                        draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed",
                        pcmd->TextureId,
                        pcmd->ClipRect.x,
                        pcmd->ClipRect.y,
                        pcmd->ClipRect.z,
                        pcmd->ClipRect.w);
                    if (show_clip_rects && ImGui::IsItemHovered()) {
                        ImRect clip_rect = pcmd->ClipRect;
                        ImRect vtxs_rect;
                        for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount; i++)
                            vtxs_rect.Add(draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
                        clip_rect.Floor();
                        overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max, IM_COL32(255, 255, 0, 255));
                        vtxs_rect.Floor();
                        overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max, IM_COL32(255, 0, 255, 255));
                    }
                    if (!pcmd_node_open) continue;
                    ImGuiListClipper clipper(pcmd->ElemCount / 3);  // Manually coarse clip
                                                                    // our print out of
                                                                    // individual vertices
                                                                    // to save CPU, only
                                                                    // items that may be
                                                                    // visible.
                    while (clipper.Step())
                        for (int prim = clipper.DisplayStart, vtx_i = elem_offset + clipper.DisplayStart * 3;
                             prim < clipper.DisplayEnd;
                             prim++)
                        {
                            char   buf[300], *buf_p = buf;
                            ImVec2 triangles_pos[3];
                            for (int n = 0; n < 3; n++, vtx_i++) {
                                ImDrawVert& v    = draw_list->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
                                triangles_pos[n] = v.pos;
                                buf_p += sprintf(buf_p,
                                    "%s %04d { pos = (%8.2f,%8.2f), uv = "
                                    "(%.6f,%.6f), col = %08X }\n",
                                    (n == 0) ? "vtx" : "   ",
                                    vtx_i,
                                    v.pos.x,
                                    v.pos.y,
                                    v.uv.x,
                                    v.uv.y,
                                    v.col);
                            }
                            ImGui::Selectable(buf, false);
                            if (ImGui::IsItemHovered())
                                overlay_draw_list->AddPolyline(triangles_pos,
                                    3,
                                    IM_COL32(255, 255, 0, 255),
                                    true,
                                    1.0f,
                                    false);  // Add triangle without AA, more readable for
                                             // large-thin triangle
                        }
                    ImGui::TreePop();
                }
                overlay_draw_list->PopClipRect();
                ImGui::TreePop();
            }

            static void NodeWindows(ImVector<ImGuiWindow*>& windows, const char* label)
            {
                if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size)) return;
                for (int i = 0; i < windows.Size; i++) Funcs::NodeWindow(windows[i], "Window");
                ImGui::TreePop();
            }

            static void NodeWindow(ImGuiWindow* window, const char* label)
            {
                if (!ImGui::TreeNode(
                        window, "%s '%s', %d @ 0x%p", label, window->Name, window->Active || window->WasActive, window))
                    return;
                NodeDrawList(window->DrawList, "DrawList");
                ImGui::BulletText("Pos: (%.1f,%.1f)", window->Pos.x, window->Pos.y);
                ImGui::BulletText("Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)",
                    window->Size.x,
                    window->Size.y,
                    window->SizeContents.x,
                    window->SizeContents.y);
                ImGui::BulletText("Scroll: (%.2f,%.2f)", window->Scroll.x, window->Scroll.y);
                if (window->RootWindow != window) NodeWindow(window->RootWindow, "RootWindow");
                if (window->DC.ChildWindows.Size > 0) NodeWindows(window->DC.ChildWindows, "ChildWindows");
                ImGui::BulletText(
                    "Storage: %d bytes", window->StateStorage.Data.Size * (int)sizeof(ImGuiStorage::Pair));
                ImGui::TreePop();
            }
        };

        ImGuiContext& g = *GImGui;  // Access private state
        Funcs::NodeWindows(g.Windows, "Windows");
        if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)", g.RenderDrawLists[0].Size)) {
            for (int i = 0; i < g.RenderDrawLists[0].Size; i++)
                Funcs::NodeDrawList(g.RenderDrawLists[0][i], "DrawList");
=======
void ImGui::ShowMetricsWindow(bool *p_open) {
  if (ImGui::Begin("ImGui Metrics", p_open)) {
    ImGui::Text("ImGui %s", ImGui::GetVersion());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("%d vertices, %d indices (%d triangles)",
                ImGui::GetIO().MetricsRenderVertices,
                ImGui::GetIO().MetricsRenderIndices,
                ImGui::GetIO().MetricsRenderIndices / 3);
    ImGui::Text("%d allocations", ImGui::GetIO().MetricsAllocs);
    static bool show_clip_rects = true;
    ImGui::Checkbox("Show clipping rectangles when hovering a ImDrawCmd",
                    &show_clip_rects);
    ImGui::Separator();

    struct Funcs {
      static void NodeDrawList(ImDrawList *draw_list, const char *label) {
        bool node_open = ImGui::TreeNode(
            draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label,
            draw_list->_OwnerName ? draw_list->_OwnerName : "",
            draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size,
            draw_list->CmdBuffer.Size);
        if (draw_list == ImGui::GetWindowDrawList()) {
          ImGui::SameLine();
          ImGui::TextColored(ImColor(255, 100, 100),
                             "CURRENTLY APPENDING"); // Can't display stats for
                                                     // active draw list! (we
                                                     // don't have the data
                                                     // double-buffered)
          if (node_open)
>>>>>>> master
            ImGui::TreePop();
          return;
        }
<<<<<<< HEAD
        if (ImGui::TreeNode("Popups", "Open Popups Stack (%d)", g.OpenPopupStack.Size)) {
            for (int i = 0; i < g.OpenPopupStack.Size; i++) {
                ImGuiWindow* window = g.OpenPopupStack[i].Window;
                ImGui::BulletText("PopupID: %08x, Window: '%s'%s%s",
                    g.OpenPopupStack[i].PopupId,
                    window ? window->Name : "NULL",
                    window && (window->Flags & ImGuiWindowFlags_ChildWindow) ? " ChildWindow" : "",
                    window && (window->Flags & ImGuiWindowFlags_ChildMenu) ? " ChildMenu" : "");
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Basic state")) {
            ImGui::Text("FocusedWindow: '%s'", g.FocusedWindow ? g.FocusedWindow->Name : "NULL");
            ImGui::Text("HoveredWindow: '%s'", g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
            ImGui::Text("HoveredRootWindow: '%s'", g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
            ImGui::Text("HoveredID: 0x%08X/0x%08X",
                g.HoveredId,
                g.HoveredIdPreviousFrame);  // Data is "in-flight" so depending
                                            // on when the Metrics window is
                                            // called we may see current frame
                                            // information or not
            ImGui::Text("ActiveID: 0x%08X/0x%08X", g.ActiveId, g.ActiveIdPreviousFrame);
            ImGui::TreePop();
=======
        if (!node_open)
          return;

        ImDrawList *overlay_draw_list =
            &GImGui->OverlayDrawList; // Render additional visuals into the
                                      // top-most draw list
        overlay_draw_list->PushClipRectFullScreen();
        int elem_offset = 0;
        for (const ImDrawCmd *pcmd = draw_list->CmdBuffer.begin();
             pcmd < draw_list->CmdBuffer.end();
             elem_offset += pcmd->ElemCount, pcmd++) {
          if (pcmd->UserCallback) {
            ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback,
                              pcmd->UserCallbackData);
            continue;
          }
          ImDrawIdx *idx_buffer = (draw_list->IdxBuffer.Size > 0)
                                      ? draw_list->IdxBuffer.Data
                                      : NULL;
          bool pcmd_node_open = ImGui::TreeNode(
              (void *)(pcmd - draw_list->CmdBuffer.begin()),
              "Draw %-4d %s vtx, tex = %p, clip_rect = "
              "(%.0f,%.0f)..(%.0f,%.0f)",
              pcmd->ElemCount,
              draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed",
              pcmd->TextureId, pcmd->ClipRect.x, pcmd->ClipRect.y,
              pcmd->ClipRect.z, pcmd->ClipRect.w);
          if (show_clip_rects && ImGui::IsItemHovered()) {
            ImRect clip_rect = pcmd->ClipRect;
            ImRect vtxs_rect;
            for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount;
                 i++)
              vtxs_rect.Add(
                  draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
            clip_rect.Floor();
            overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max,
                                       IM_COL32(255, 255, 0, 255));
            vtxs_rect.Floor();
            overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max,
                                       IM_COL32(255, 0, 255, 255));
          }
          if (!pcmd_node_open)
            continue;
          ImGuiListClipper clipper(pcmd->ElemCount / 3); // Manually coarse clip
                                                         // our print out of
                                                         // individual vertices
                                                         // to save CPU, only
                                                         // items that may be
                                                         // visible.
          while (clipper.Step())
            for (int prim = clipper.DisplayStart,
                     vtx_i = elem_offset + clipper.DisplayStart * 3;
                 prim < clipper.DisplayEnd; prim++) {
              char buf[300], *buf_p = buf;
              ImVec2 triangles_pos[3];
              for (int n = 0; n < 3; n++, vtx_i++) {
                ImDrawVert &v =
                    draw_list
                        ->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
                triangles_pos[n] = v.pos;
                buf_p += sprintf(buf_p, "%s %04d { pos = (%8.2f,%8.2f), uv = "
                                        "(%.6f,%.6f), col = %08X }\n",
                                 (n == 0) ? "vtx" : "   ", vtx_i, v.pos.x,
                                 v.pos.y, v.uv.x, v.uv.y, v.col);
              }
              ImGui::Selectable(buf, false);
              if (ImGui::IsItemHovered())
                overlay_draw_list->AddPolyline(
                    triangles_pos, 3, IM_COL32(255, 255, 0, 255), true, 1.0f,
                    false); // Add triangle without AA, more readable for
                            // large-thin triangle
            }
          ImGui::TreePop();
>>>>>>> master
        }
        overlay_draw_list->PopClipRect();
        ImGui::TreePop();
      }

      static void NodeWindows(ImVector<ImGuiWindow *> &windows,
                              const char *label) {
        if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size))
          return;
        for (int i = 0; i < windows.Size; i++)
          Funcs::NodeWindow(windows[i], "Window");
        ImGui::TreePop();
      }

      static void NodeWindow(ImGuiWindow *window, const char *label) {
        if (!ImGui::TreeNode(window, "%s '%s', %d @ 0x%p", label, window->Name,
                             window->Active || window->WasActive, window))
          return;
        NodeDrawList(window->DrawList, "DrawList");
        ImGui::BulletText("Pos: (%.1f,%.1f)", window->Pos.x, window->Pos.y);
        ImGui::BulletText("Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)",
                          window->Size.x, window->Size.y,
                          window->SizeContents.x, window->SizeContents.y);
        ImGui::BulletText("Scroll: (%.2f,%.2f)", window->Scroll.x,
                          window->Scroll.y);
        if (window->RootWindow != window)
          NodeWindow(window->RootWindow, "RootWindow");
        if (window->DC.ChildWindows.Size > 0)
          NodeWindows(window->DC.ChildWindows, "ChildWindows");
        ImGui::BulletText("Storage: %d bytes",
                          window->StateStorage.Data.Size *
                              (int)sizeof(ImGuiStorage::Pair));
        ImGui::TreePop();
      }
    };

    ImGuiContext &g = *GImGui; // Access private state
    Funcs::NodeWindows(g.Windows, "Windows");
    if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)",
                        g.RenderDrawLists[0].Size)) {
      for (int i = 0; i < g.RenderDrawLists[0].Size; i++)
        Funcs::NodeDrawList(g.RenderDrawLists[0][i], "DrawList");
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Popups", "Open Popups Stack (%d)",
                        g.OpenPopupStack.Size)) {
      for (int i = 0; i < g.OpenPopupStack.Size; i++) {
        ImGuiWindow *window = g.OpenPopupStack[i].Window;
        ImGui::BulletText(
            "PopupID: %08x, Window: '%s'%s%s", g.OpenPopupStack[i].PopupId,
            window ? window->Name : "NULL",
            window && (window->Flags & ImGuiWindowFlags_ChildWindow)
                ? " ChildWindow"
                : "",
            window && (window->Flags & ImGuiWindowFlags_ChildMenu)
                ? " ChildMenu"
                : "");
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Basic state")) {
      ImGui::Text("FocusedWindow: '%s'",
                  g.FocusedWindow ? g.FocusedWindow->Name : "NULL");
      ImGui::Text("HoveredWindow: '%s'",
                  g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
      ImGui::Text("HoveredRootWindow: '%s'",
                  g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
      ImGui::Text("HoveredID: 0x%08X/0x%08X", g.HoveredId,
                  g.HoveredIdPreviousFrame); // Data is "in-flight" so depending
                                             // on when the Metrics window is
                                             // called we may see current frame
                                             // information or not
      ImGui::Text("ActiveID: 0x%08X/0x%08X", g.ActiveId,
                  g.ActiveIdPreviousFrame);
      ImGui::TreePop();
    }
  }
  ImGui::End();
}

//-----------------------------------------------------------------------------

// Include imgui_user.inl at the end of imgui.cpp to access private
// data/functions that aren't exposed.
// Prefer just including imgui_internal.h from your code rather than using this
// define. If a declaration is missing from imgui_internal.h add it or request
// it on the github.
#ifdef IMGUI_INCLUDE_IMGUI_USER_INL
#include "imgui_user.inl"
#endif

//-----------------------------------------------------------------------------
