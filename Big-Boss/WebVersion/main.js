

		var Block = Class.create( function( top, left, width, height ){
			this.top = top;
			this.left = left;
			this.border = false;
		},

		{
			init: function(){

			},

			changeBg: function( bg, framesNum, combo, position ){
				
				var _oWidth = this.width;

				this.width =  Util.imgObj[ bg ].width / framesNum;

				this._oWidth = ( this.width - _oWidth ) * Config.map.spiritZoom;

				if ( this.direction === -1 ){
					this.left = this.left - this._oWidth;
				}
				
				if ( ( this.currState === 'somesault_up' ) &&  this.border === 'right' ){
					this.left = this.left - 60;	
				}

				if ( !combo ){

					var _oHeight = this.height;
	
					this.height = Util.imgObj[ bg ].height ;
	
					this._oHeight = ( this.height - _oHeight ) * Config.map.spiritZoom;	
					this.top = ( this.top - this._oHeight );
				}else{
					
				}

				this.position = position || 0;

			},

			crossBorder: function( left ){
				if ( !Map || !Map.getMaxX() ) return left;
				var maxX =  Map.getMaxX();
				var right = this.state === 'jump_back' ? 130 : 85;
				if ( left < 15 ){
					this.border = 'left';
					return 15;
				}else if ( left + this.width > maxX - right ){
					this.border = 'right';
					return maxX - this.width - right;
				}
				this.border = false;
				return left;
			}

		})