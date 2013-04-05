package com.serwylo.brewdiary.data
{

public class Bottle
{

	public var bottleType:BottleType;
	public var count:Number;
	public var used:Number;

	public function get remaining():Number { return this.count - used; }
}

}
