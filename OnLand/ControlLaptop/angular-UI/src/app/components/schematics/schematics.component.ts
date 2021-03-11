import { Component, OnInit, ViewEncapsulation } from '@angular/core';
import * as go from 'gojs';
import { DataSyncService, DiagramComponent, PaletteComponent } from 'gojs-angular';

@Component({
	selector: 'app-schematics',
	templateUrl: './schematics.component.html',
	styleUrls: ['./schematics.component.css'],
	encapsulation: ViewEncapsulation.None
})
export class SchematicsComponent implements OnInit {

	constructor() { }
	dia: go.Diagram;
	ngOnInit(): void {
	}

	public initDiagram(): go.Diagram {

		const $ = go.GraphObject.make;
		this.dia = $(go.Diagram, {
			'undoManager.isEnabled': false, // must be set to allow for model change listening
			// 'undoManager.maxHistoryLength': 0,  // uncomment disable undo/redo functionality
			initialContentAlignment: go.Spot.None,
			model: $(go.GraphLinksModel,
				{
					linkKeyProperty: 'key' // IMPORTANT! must be defined for merges and data sync when using GraphLinksModel
				}
			)
		});

		// define the Node template
		this.dia.nodeTemplate =
			$(go.Node, 'Auto',
				{
					toLinkable: false, fromLinkable: false,
					click: (e, obj) => {
						let loc = obj.part.location;
						var docloc = this.dia.transformDocToView(loc);
						console.log("Selected node location,\ndocument coordinates: " + loc.x.toFixed(2) + " " + loc.y.toFixed(2) + "\nview coordinates: " + docloc.x.toFixed(2) + " " + docloc.y.toFixed(2));
					}
				},
				new go.Binding("location", "loc", go.Point.parse),
				new go.Binding('fill', 'color'),
				new go.Binding('scale', 'scale'),
				$(go.Panel, "Spot",
					$(go.Picture, new go.Binding("source", "img"), new go.Binding("desiredSize", "size"),
					), $(go.Shape,
						{
							width: 0.1, height: 0.1, portId: "Top",
							fromLinkable: false, stroke: null, fill: "pink", alignment: go.Spot.Top,
						}),
					$(go.Shape,
						{
							width: 0, height: 0, portId: "Bottom",
							fromLinkable: false, stroke: null, fill: "transparent", alignment: go.Spot.Bottom,
						}),
					$(go.Shape,
						{
							width: 0, height: 0, portId: "Left",
							fromLinkable: false, stroke: null, fill: "transparent", alignment: go.Spot.Left,
						}),
					$(go.Shape,
						{
							width: 0, height: 0, portId: "Right",
							fromLinkable: false, stroke: null, fill: "transparent", alignment: go.Spot.Right,
						})

				)
			);
		this.dia.linkTemplate = 
		$(go.Link,
			{ routing: go.Link.Orthogonal },
			$(go.Shape,
				new go.Binding("stroke", "color"),  // shape.stroke = data.color
				new go.Binding("strokeWidth", "thick")));

		this.dia.model = $(go.GraphLinksModel,
			{
				linkFromPortIdProperty: "fromPort",  // required information:
				linkToPortIdProperty: "toPort"
			});

		return this.dia;
	}

	public diagramNodeData = [
		{ key: 'auger', group: "auger_group", scale: 2.5, img: '../../../assets/Auger.svg', color: "red", loc: "643.50 -54.17" },
		{ key: 'P1_1', group: "auger_group", scale: 2.5, img: '../../../assets/P1.svg', loc: "696.50 -11.77" },
		{ key: 'P1_2', group: "auger_group", scale: 2.5, img: '../../../assets/P1.svg', loc: "696.50 41.83" },
		{ key: 'Relief_Valve_1', group: "auger_group", scale: 2.5, img: '../../../assets/Relief_Valve_Straight.svg', loc: "756.54 41.83" },
		{ key: 'Motor_small_1', group: "auger_group", scale: 2.5, img: '../../../assets/Motor_small.svg', loc: "822.50 -19.17" },
		{ key: 'auger_group', isGroup: true },
		{ key: 'Delta' }
	];
	public diagramLinkData = [
		{ from: 'Motor_small_1', to: 'P1_1', color: 'red', fromPort: "Left", toPort: "Right" },
		{ from: 'Relief_Valve_1', to: 'Motor_small_1', color: 'green', fromPort: "Right", toPort: "Right" },
		{ from: 'P1_2', to: 'Relief_Valve_1', color: 'green', fromPort: "Right", toPort: "Right" },
		{ from: 'P1_1', to: 'Relief_Valve_1', color: 'red', fromPort: "Right", toPort: "Top", "points":[511,280] },

	];
	public diagramDivClassName: string = 'myDiagramDiv';
	public diagramModelData = { prop: 'value' };
	public skipsDiagramUpdate = false;

	// When the diagram model changes, update app data to reflect those changes
	public diagramModelChange = function (changes: go.IncrementalData) {
		// when setting state here, be sure to set skipsDiagramUpdate: true since GoJS already has this update
		// (since this is a GoJS model changed listener event function)
		// this way, we don't log an unneeded transaction in the Diagram's undoManager history
		this.skipsDiagramUpdate = true;

		this.diagramNodeData = DataSyncService.syncNodeData(changes, this.diagramNodeData);
		this.diagramLinkData = DataSyncService.syncLinkData(changes, this.diagramLinkData);
		this.diagramModelData = DataSyncService.syncModelData(changes, this.diagramModelData);
	};
	
	handleInspectorChange() {
		this.dia.commit(m => {
		let node1=m.findNodeForKey('Relief_Valve_1')
		let node2=m.findNodeForKey('Motor_small_1')
		node1.findLinksBetween(node2).each(x => {
			console.log('x')
			this.dia.model.set(x.data, "color", "orange")
		})
	})
}
	

}