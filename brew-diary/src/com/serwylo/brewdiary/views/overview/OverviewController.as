package com.serwylo.brewdiary.views.overview
{

import com.serwylo.brewdiary.core.Controller;
import com.serwylo.brewdiary.core.View;
import com.serwylo.brewdiary.data.Brew;
import com.serwylo.brewdiary.data.Note;
import com.serwylo.brewdiary.data.Review;
import com.serwylo.brewdiary.data.SgReading;
import com.serwylo.brewdiary.events.DataChangeEvent;
import com.serwylo.brewdiary.forms.NewBrewForm;
import com.serwylo.brewdiary.services.BrewService;
import com.serwylo.brewdiary.services.Database;
import com.serwylo.brewdiary.services.StatusService;
import com.serwylo.brewdiary.views.brew.BrewController;

import flash.desktop.NativeApplication;
import flash.events.Event;

import flash.events.SQLErrorEvent;
import flash.events.SQLEvent;

import mx.collections.ArrayCollection;
import mx.controls.Alert;
import mx.core.FlexGlobals;
import mx.rpc.AbstractOperation;
import mx.rpc.events.FaultEvent;
import mx.rpc.events.ResultEvent;
import mx.rpc.soap.LoadEvent;
import mx.rpc.soap.WebService;

import spark.components.Application;

public class OverviewController extends Controller
{

	private var _view:OverviewView;
	private var _model:OverviewModel;

	public function OverviewController()
	{
		this._model = new OverviewModel();
		this._view = new OverviewView();
		this._view.addEventListener( DataChangeEvent.DATA_CHANGE, this.onDataChange );
		this._view.addEventListener( OverviewView.NEW_BATCH, this.onNewBatch );
		this._view.setup( this._model );

		// Must init first, or else we don't get the events...
		Database.instance.addEventListener( SQLEvent.OPEN, this.onDatabaseConnected );
		Database.instance.addEventListener( SQLErrorEvent.ERROR, this.onDatabaseConnectError );
		Database.instance.init();
	}

	private function onDatabaseConnected( event:SQLEvent ):void
	{
		this._model.statusList = new ArrayCollection( StatusService.getStatusList() );
		this._model.brewList = new ArrayCollection( BrewService.getBrewList() );
	}

	private function onDatabaseConnectError( event:SQLErrorEvent ):void
	{
		Alert.show( "Error connecting to database (" + event.errorID + "): " + event.error, "Fatal Error", Alert.OK,  null, NativeApplication.nativeApplication.exit );
	}

	private function onDataChange( event:DataChangeEvent ):void
	{
		switch( event.kind )
		{
			case 'selectedBatch':
				if ( event.data == null )
				{
					this._model.batchController = null;
				}
				else
				{
					if ( this._model.batchController == null )
					{
						this._model.batchController = new BrewController();
					}
					this._model.batchController.batch = event.data as Brew;
				}
			break;
		}
	}

	public override function get view():View
	{
		return this._view;
	}

	private function onNewBatch( event:Event ):void
	{
		var form:NewBrewForm = NewBrewForm.show( this._model.statusList );
		form.addEventListener( Event.SELECT, this.onNewBrewSubmitted );
	}

	private function onNewBrewSubmitted( event:Event ):void
	{
		var form:NewBrewForm = event.target as NewBrewForm;
		var brew:Brew = new Brew();
		brew.brewLabel = form.typeValue;
		brew.startDate = form.startDateValue;
		brew.status = form.statusValue;
		BrewService.saveBrewService( brew );
		this._model.brewList.addItem( brew );
	}

}

}
