package com.serwylo.brewdiary.data
{
public class Review
{

	[Embed(source="../icons/award_star_gold_3.png")]
	public static const IMG_GOOD:Class;

	[Embed(source="../icons/award_star_silver_2.png")]
	public static const IMG_OK:Class;

	[Embed(source="../icons/award_star_bronze_1.png")]
	public static const IMG_BAD:Class;

	public var shortDescription:String;
	public var longDescription:String;
	public var rating:Number;
	public var createdDate:Date = new Date();

	public function Review()
	{
	}

	public function getIcon():Class
	{
		if ( this.rating <= 3 )
		{
			return IMG_BAD;
		}
		else if ( this.rating <= 6 )
		{
			return IMG_OK;
		}
		else
		{
			return IMG_GOOD;
		}
	}

}

}
