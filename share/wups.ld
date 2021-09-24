SECTIONS
{
    .fimport_homebrew_wupsbackend ALIGN(16) : {
     KEEP ( *(.fimport_homebrew_wupsbackend) )
     *(.fimport_homebrew_wupsbackend.*)
    } > loadmem
    .dimport_homebrew_wupsbackend ALIGN(16) : {
     KEEP ( *(.dimport_homebrew_wupsbackend) )
     *(.dimport_homebrew_wupsbackend.*)
    } > loadmem

   .wups.meta : {
	  *(.wups.meta*)
      KEEP(*(.wups.meta*))
   } > datamem
   
   .wups.load : {
	  *(.wups.load*)
      KEEP(*(.wups.load*))
   } > datamem
   
   .wups.hooks : {
	  *(.wups.hooks*)
      KEEP(*(.wups.hooks*))
   } > datamem
}