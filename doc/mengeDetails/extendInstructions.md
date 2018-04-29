Extending the Simulator                {#page_extension}
======================

%Menge has been designed with the purpose of being extensible.  To add additional functionality, the user simply implements a plug-in (see @ref sec_plugin) with one or more plug-in elements.  If a plug-in element is properly implemented, the %Menge program will automatically detect and load the plugin and register its elements.  These elements will extend the features that can be parsed in the XML project specifications and introduce new behaviors and functionality to the system.  The plug-in elements which can be defined in a plug-in are the following:

- Pedestrian models (see @ref page_PedModel)
- (TODO): Behavior finite state machine velocity components (see @ref page_VelComp)
- (TODO): Behavior finite state machine transitions (see @ref page_Transition)
- (TODO): Behavior finite state machine actions (see @ref page_Actions)
- (TODO): Spatial query elements (see @ref page_SpatialQuery)
- (TODO): Elevation elements (see @ref page_Elevation)

@section sec_plugin Implementing a Plug-in

Ultimately, all new elements introduced to the system must be incorporated in one or more plug-ins, implemented as shared libraries (dlls on windows or so files on linux).  The main plug-in architecture is quite simple.  To make a %Menge-compatible plug-in, one simply has to write two functions:

- One function which reports the plug-ins name.
- One function which registers the various plug-in elements to %Menge's PluginEngine.
	
The details for writing a shared library depend on the operating system on which the code is compiled.  We suggest the following code snippet be included in your main plug-in definition code to make your code compatible with both windows and linux.

    #if defined(_MSC_VER)
      #define PLUGIN_EXPORT __declspec(dllexport)
    #elif defined(__GNUC__)
      #define PLUGIN_EXPORT __attribute__ ((visibility ("default")))
    #endif

The plug-in name function takes the following form:

		extern "C" PLUGIN_EXPORT const char * getName() {
			return "Plug-in name";
		}
		
Upon loading the plug-in, %Menge will report the name of the plug-in being loaded.  This is the run-time reality check that your plug-in has been successfully loaded.

When the plug-in is loaded, it needs to report the system the nature of the elements the plug-in defines.  %Menge provides a pointer to an instance of PluginEngine.  The PluginEngine class has a function which the plug-in can pass an instance of the appropriate registration class.  Each plug-in element comes with its own registration class (see the plug-in element pages for details).

The function will look like this:

    extern "C" PLUGIN_EXPORT void registerPlugin( PluginEngine * engine ) {
      engine->register_________( new ___________() );
    }

The blank spaces should be replaced by the particular plug-in entities.

