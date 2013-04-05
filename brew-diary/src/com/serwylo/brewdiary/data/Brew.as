/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 29/12/10
 * Time: 7:39 PM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.data
{
import mx.collections.ArrayCollection;
import mx.collections.ArrayList;

[Bindable]
public class Brew
{


	[Embed(source="../icons/exclamation.png")]
	public static const IMG_EMPTY:Class;

	[Embed(source="../icons/clock.png")]
	public static const IMG_FERMENTING:Class;

	[Embed(source="../icons/clock_add.png")]
	public static const IMG_BOTTLE_FERMENTING:Class;

	[Embed(source="../icons/award_star_grey.png")]
	public static const IMG_PENDING_REVIEW:Class;

	public var brewSerial:Number;
	public var brewLabel:String;
	public var startDate:Date;
	public var initialFermentationDate:Date;
	public var bottledDate:Date;
	public var status:Status;

	public function getIcon():Class
	{
		if ( this.status.statusSerial == Status.STATUS_EMPTY )
		{
			return null;
		}
		return null;
	}
	
	public var sgReadings:ArrayCollection;
	public var notes:ArrayCollection;
	public var reviews:ArrayCollection;
	public var bottles:ArrayCollection;

	public function Brew()
	{

	}

	public function get mostRecentSg():SgReading
	{
		var newest:SgReading;
		for each ( var reading:SgReading in this.sgReadings )
		{
			if ( newest == null || reading.readingDate.time > newest.readingDate.time )
			{
				newest = reading;
			}
		}
		return newest;
	}
	
}

}
