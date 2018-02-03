# Plugin system for the Wii U. (WIP)

I decided to make this entirely public, so everbody can contribute.

# Goal

The goal is provide a mechanism to modifiy your Wii U experience in a modular way.
Currently each features has it's own application (TCPGecko, SwapDRC, HIDtoVPAD, SDCafiine) and you can only start one applicationa the same time.
This means, you can't mix any of these features. One way would be to create an own App for each feature combination (like geckiine) or create one app that has all features.
Both approaches have the same disadvantage, it's nearly impossible (and time consuming) to support everthing requested/possible.

The idea is now:
- Each feature will be deployed as an individual module/plugin
- These modules will be loaded with ONE generic loader
- And can be mixed in any way
- A GUI can (de)activate them without any new compiling
- Everything can be handled easily on the console itself
	
# Planned features

## Short-term goals
Possbile to create a plugin which:
* can override existing system function
  * provide a hook function which will be called
  * on each start of an application
  * everytime a application will be closed
  * on each frame
* can be combined with other plugins

With an simple, generic loader which:
* Loads the plugins from the SDCard
* Combines them and use them ALL at the same time.	

General:
* Port some currently available tools.
	
## Mid-term goals
General:
- Port all common applications like TCPGecko etc.

Loader:
- Allow to patch the currently loaded RPX
  - This would lead game binaries
	
More advanced plugin loader:
- Create configuration system:
  - (de)activate certain plugin only in set applications.
  - (de)activate plugins completly without removing them from the SDCard.
		
## Long-term goals
Even more advanced plugin loader:
- Downloading plugins directly on the console
- Updatin existing plugins
- Share configurations with other user
- Allow the plugin to have an own configuration window.


# Technical problems

To achieve this, serveral "problems" need to be resolved (feature need to be implemented).
This is just an short overview. Look at the issued for detailed information.
	
### plugin	
- Create an stable interface the plugins will use.
  - with hooks for
  - application start
    - application end
    - each frame
    - functions
  - some way to configure them?		
- Prodivde the plugin as a .elf?
  - Keep the file ending?
  - Compress/zip it?
- How to add meta information?
  - Compatibility with
    - Plugin loader
    - Wii U FW
  - Information about the plugin
    - Author(s)
    - Version
    - Description
- Linking / building
  - How to link the files
  - Write a proper (base) makefile
  - Is position independent code possible?
    - Is it reliable?
  - If not, how does elf relocation work?

## function patching
- (elf relocation)
- use the information from the plugin
  - parse it
    - HOW to store it?
- save the patching order
- add basic disable/enable mechanism
	
## general
- provided general hooking points
- How to store the plugins
  - Where is enough, unused space
- keep tracking of the plugins
- simple gui?
- provide functions?
  - like libfat, libiosu activated
  - kernel copy function
- pass information into the plugins
  - Running on which FW
  - arguments?
  - ???
	

# Similar projects

## function-patcher-example
Platform: Wii U
Notes: TODO
https://github.com/Maschell/function-patcher-example

## COSSubstrate
Platform: Wii U
Notes: TODO
https://github.com/QuarkTheAwesome/COSSubstrate

## brainslug-wii
Platform: Wii
Notes: TODO
https://github.com/Chadderz121/brainslug-wii

## substitute
Platform: IOS
Notes: TODO
https://github.com/comex/substitute

