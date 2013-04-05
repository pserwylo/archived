<?php 
/**
 * Here is my comment...  
 * @access 0 Here we go.  
 * @log info Bleh.  
 */ 
function singleLog( ) 
{ 
    return "Single Log ( access == 0 )"; 
} 

/** 
 * Here is my comment...
 * @access 1 Here we go.
 * @log info Does some stuff.
 * @log info Oh, it also does some other stuff.
 * @log info And finally, it does nothing.
 */
function multiLog( )
{
    return "Multi Log ( access == 1 )";
}

function doesntLog( )
{
   return "Doesn't log" ;
}

function aophpLog( $function, $attributeValue )
{
    echo "[Logging: $function, $attributeValue]\n";
    return true;
}

function aophpMultiLog( $function, $attributeValuesArray )
{
    echo "[Logging: $function, " . implode( ",", $attributeValuesArray ) . "]\n";
    return true;
}

function aophpAuthorize( $function, $attributeValue )
{
    $parts = split( " ", $attributeValue );
    $access = $parts[0];
    echo "[Checking access: " . $function . ", " . $access . "]";
    return ( $access == 1 );
}

// aophp_add_hook( "log", "aophpLog", 0, 0 );
// aophp_add_hook( "log", "aophpMultiLog", 1, 0 );

aophp_add_hook( "access", "aophpAuthorize", 0, 1 );

echo singleLog( );
echo "\n\n";
echo multiLog( );
echo "\n\n";
echo doesntLog( );
echo "\n\n";
