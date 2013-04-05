package com.serwylo.brewdiary.events
{

import flash.events.Event;

public class DataChangeEvent extends Event
{

	public static const DATA_CHANGE:String = "dataChange";

	public function get kind():String { return this._kind; }
	private var _kind:String;

	public function get data():Object { return this._data; }
	private var _data:Object;

	public function DataChangeEvent( kind:String, data:Object, bubbles:Boolean = false, cancelable:Boolean = false )
	{
		super( DATA_CHANGE, bubbles, cancelable );
		this._data = data;
		this._kind = kind;
	}

	public override function clone():Event
	{
		return new DataChangeEvent( this.kind, this.data, this.bubbles, this.cancelable );
	}
	
}

}
