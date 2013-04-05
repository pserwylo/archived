package com.serwylo.brewdiary.data
{

public class Status
{

	public static const STATUS_NOT_STARTED:Number = 1;
	public static const STATUS_INITIAL_FERMENTATION:Number = 2;
	public static const STATUS_BOTTLE_FERMENTATION:Number = 3;
	public static const STATUS_BOTTLED_AGEING:Number = 4;
	public static const STATUS_READY:Number = 5;
	public static const STATUS_EMPTY:Number = 6;

	public var statusLabel:String;
	public var statusSerial:Number;

	/**
	 * Only after a beer has been bottled can it be reviewed.
	 * We will not wait for it to be "Ready", rather just bottled.
	 * This way, we can write reviews when we are testing to see if it IS ready.
	 */
	public function get canReview():Boolean
	{
		return this.statusSerial == STATUS_BOTTLED_AGEING || this.statusSerial == STATUS_READY || this.statusSerial == STATUS_EMPTY;
	}


	public function get isBottled():Boolean
	{
		return this.statusSerial == STATUS_BOTTLE_FERMENTATION || this.statusSerial == STATUS_BOTTLED_AGEING || this.statusSerial == STATUS_READY || this.statusSerial == STATUS_EMPTY;
	}
}

}
