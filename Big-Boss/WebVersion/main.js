

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


	var Spirit = Block.subClass( function( config ){
			this.config = config;
			this.name = config.name;
			this.states = config.states;
			this.f_top = 0;
			this.f_left = 0;
			this.top = 0;
			this.left = 0;
			this.width = 0;
			this.height = 0;
			this.bottom = 0;
			this.direction = 1;
			this.state = null;
			this.currState = null;
			this.enemy = null;
			this.animate = null;	
			this.animateDirection = [ 0, 0 ];
			this.frames = null;		
			this.shadow = null;	 
			this.queue = null;		
			this.keyManage = null;
			this.collision = null;
			this.attack = null;   
			this.waveBoxing = null; 
			this.status = null;   
			this.bloodBar = null;
			this.statusManage = null;
			this.stageManage = null;
		},

		{
			init: function( f_left, f_top, direction ){
				this.f_left = f_left;
				this.f_top = f_top;
				this.f_height = Util.imgObj[ this.states[ this.states.default ].bg ].height;
				this.direction = direction;
				this.frames = this.implement( 'SpiritFrames' );
				this.shadow = this.implement( 'Shadow');
				this.animate = this.implement( 'Animate' );
				this.queue = this.implement( 'Queue' );
				this.lock = this.implement( 'Lock' );
				this.movelock = this.implement( 'Lock' );
				this.keyManage = this.implement( 'KeyManage', this.config.keyMap );
				this.collision = this.implement( 'Collision' );  
				this.attack = Fighter.getInstance( this ).init();
				this.waveBoxing = WaveBoxing.getInstance( this ).init();
				this.statusManage = this.implement( 'StatusManage' );
				this.stageManage = this.implement( 'Stage' );
				this.event();

				var currState = this.states[ this.states.default ];

				this.changeBg( currState.bg, currState.framesNum );

				this.play( this.states.default );

				this.animate.moveto( f_left, f_top );

				return this;
			},

			setEnemy: function( enemy ){
				this.enemy = enemy;
			},

			play: function( state, force ){
			
				if ( this.statusManage.isCrouch() && ( state === 'wait' || state === 'force_wait' ) ){  
					return this.play( 'stand_up', true );
				}

				if ( this.jump_combo( state ) && state !== 'force_wait' ) return;  //������ϼ�
			

				if ( this.defense( state ) ) return;  //�Ƿ��ڷ���״̬
				
				var lock_level = Config.play[ state ].lock || 0;			
				
				var old_lock_leval = this.lock.getLevel() || 0;
				

				if ( !force ){
	
					if ( state === this.state || ( this.lock.locked() && old_lock_leval >= lock_level ) ) {	
						return;
					}

				}
				
				
				this.lock.lock( lock_level );   //����, �����ӳ�.

				this.queue.clean();

				var composes = Config.play[ state ].compose;  //���states

				for ( var i = 0, c; c = composes[i++]; ){
					var s = Util.copy( this.states[ c ] );
					s.currState = c;
					this.queue.add( s );
				}

				this.state = state;

				if ( state === 'light_wave_boxing' || state === 'heavy_wave_boxing' ){
					if ( this.waveBoxing.firing ){
						this.queue.clean();
						this.lock.lock(0);
						return this.play( state === 'light_wave_boxing' ? 'light_boxing' : 'heavy_boxing' );
					}
					this.waveBoxing.start( this.direction, state === 'light_wave_boxing' ? this.states[ 'light_wave' ] : this.states[ 'heavy_wave' ] );
				}

				this.fireFrames();
				
				this.animate.event.fireEvent( 'playStart' );

			},


			jump_combo: function( state ){   //��Ծ����ϼ���

				if ( !this.state || !this.statusManage.isJump() ) return false;

				if ( state === 'jump_fall_down' || state === 'jump_dead' || state === 'after_dead2' || state === 'heavy_attacked_fall_down' || state === 'crouch_kick_attacked_fall' ){
					this.frames.combo_attack.stop();
					return false;
				}

				var combo = this.states.combo && this.states.combo[ this.state + '_' + state ];

				if ( !combo ) return true;

				var flag = this.frames.combo_attack.start( combo.bg, combo.framesNum, combo.repeat, combo.afterFrame );

				this.statusManage.set_attack_type( combo.attack_type );
				
				var attack_config = combo.attack_config;
	
				this.statusManage.set_attack_power( combo.attack_power );

				
				if ( flag ){   //�Ƿ����ȭ�Ѿ�����

					this.attack.stickStart( attack_config, combo.effect_position, combo.sound );

					var self = this;

					this.frames.combo_attack.done( function(){
						self.statusManage.set_attack_type( 0 );
						self.attack.stop();
					})
				}

				return true;

			},

			defense: function( state ){

				if ( state === 'back' ){
					if ( this.enemy.waveBoxing.firing || ( this.enemy.statusManage.get().attack_type === 'attack' && this.statusManage.get().enemy_distance_type !== 'furthest' ) ){
						if ( this.statusManage.get().attack_type === 'wait' ){
							this.play( 'stand_up_defense' );   //����״̬
							this.statusManage.set_attack_type( 1 );
						}
						return true;
					}
				}

				else if ( state === 'stand_crouch_defense' ){
					if ( !this.enemy.waveBoxing.firing && ( this.enemy.statusManage.get().attack_type !== 'attack' || this.statusManage.get().enemy_distance_type === 'furthest' ) ){
						this.play( 'crouch' );   //����״̬
						this.statusManage.set_attack_type( 0 );
						return true;
					}
						
				}
				
			},


			fireFrames: function(){

				var state = this.queue.dequeue();

				if ( !state ){   //������û�ж�����ʱ��

					var old_dir = this.direction;

					var new_direction = this.left > ( this.enemy && this.enemy.left ) ? -1 : 1;

					if ( new_direction !== old_dir ){
						if ( this.border ){
							this.left = this.left + new_direction * 18;
						}else{
							this.left = this.left + new_direction * 105;
						}
					}
		
					this.direction = this.left > ( this.enemy && this.enemy.left ) ? -1 : 1;  //��������
					
					this.animate.mirror( this.direction );  //������
					this.keyManage.mirror( this.direction );   //���̾���
					if ( this.statusManage.isCrouch()  ){  //���µ�״̬վ����.
						return this.play( 'stand_up', true );
					}else{
						this.queue.add( this.states[ this.states.default ] );
					}
					this.lock.lock( 0 );
					this.state = this.states.default;
					this.frames.combo_attack.stop();

					if ( this.attack.animate_type === 'stick' ){
						this.attack.stop();	
					}

					return this.fireFrames();
				}

				if ( state !== this.state ){
					this.changeBg( state.bg, state.framesNum, null, state.position );
				}


				if ( state.easing[ 1 ] === null ){
					var top = this.f_top - this.top + ( this.f_height - this.height ) * Config.map.spiritZoom;
				}


				this.currState = state.currState;
				
				this.frames.start( state.bg, state.framesNum, state.easing[2], state.repeat, state.position, this.direction );

				this.animate.start( state.easing[0] * this.direction, top || state.easing[1], state.easing[2] * Config.fps * state.framesNum, state.easing[3] );
				
				var _left = state.easing[0] * this.direction, _top = top || state.easing[1];
				
				this.animateDirection = [ _left === 0 ? 0 : _left / Math.abs( _left ), _top === 0 ? 0 : _top / Math.abs( _top )  ]; 

				this.statusManage.set_attack_type( state.attack_type || 0 );
	
				this.statusManage.set_attack_power( state.attack_power || [] );

				if ( state.attack_type > 0 || this.statusManage.isJump() ){
					//this.frames.checkZindex();  //�����㼶
				}

				if ( state.attack_type === 2 ){
					var attack_config = state.attack_config, effect_position = state.effect_position;
					attack_config && this.attack.start( attack_config[0], attack_config[1], [ attack_config[ 2 ] * this.direction, attack_config[ 3 ], attack_config[ 4 ], attack_config[ 5 ], attack_config[ 6 ], attack_config[ 7 ], attack_config[ 8 ] ], effect_position, state.sound );
				}

				if ( state.specialSound ){
					this.attack.audio.play( state.specialSound );
				}

				if ( this.currState !== 'wait' ){
					this.enemy.animate.event.fireEvent( 'stopPush' );
				}

				this.animate.event.fireEvent( 'framesStart' );
				

			},


			event: function( type ){

				var self = this;

				this.frames.event.listen( 'framesDone', function(){


					if ( self.currState === 'crouch_kick_attacked_fall' || self.currState === 'after_heavy_attacked_fall_down' || self.currState === 'dead' ){
						self.attack.audio.play( 'sound/fall.mp3' );	
					}

					if ( self.currState === 'somesault_up' ){
						self.statusManage.setInvincible( 100 );  //��������ʱ������޵�.
					}
					self.fireFrames();
				})

				this.frames.event.listen( 'framesStart', function(){

					self.animate.unlock();
					
					if ( self.state === 'wait' ){
						self.animate.correct();	  //����λ��
					}
					
				})

				this.frames.event.listen( 'frameStart', function(){

					if ( self.state === 'wait' || self.state === 'crouch' ){
						self.direction = self.left > ( self.enemy && self.enemy.left ) ? -1 : 1;  //��������	
					}

					self.collision.check();      //�����ײ
					self.animate.move();   //ÿ֡��ʼǰ����λ��.
					self.shadow.moveto( Map.height - 73, self.statusManage.isJump() && self.direction === -1 ? self.left + ( self.width - 62 ) * Config.map.spiritZoom : self.left, self.statusManage.isJump() ? 62 : self.width, self.direction );  //�ƶ���Ӱ
					self.statusManage.check_enemy_distance( self.enemy );


				})


				this.animate.event.listen( 'stopPush', function(){
					self.states.forward.easing[ 0 ] = 150;
				});

				
				this.animate.event.listen( 'frameStart', function(){

					var d = 0;

					self.enemy.animate.event.removeListen( 'frameDone' );

					if ( ( self.border === 'left' && self.animateDirection[ 0 ] === -1 ) || ( self.border === 'right' && self.animateDirection[ 0 ] === 1 ) ){

						self.enemy.stageManage.start();

						if ( self.statusManage.get().enemy_distance_type === 'furthest' ) return;

							self.stageManage.scroll( self.border );

							self.enemy.animate.event.listen( 'frameDone', function(){

								if ( self.enemy.animateDirection[ 0 ] === 0 && self.enemy.animateDirection[ 1 ] === 0 ){
									self.stageManage.pushEnemy();
								}else{
									if ( self.stageManage.isScrolling() ){
										self.enemy.animate.stagePush( self.stageManage.isScrolling() );
									}else{
										self.enemy.animate.stopStagePush();		
									}
								}
							}, 1 )

					}else{
						self.stageManage.stop();
					}

				});
				


				this.collision.event.listen( 'affirm', function( obj, dir ){

					if ( obj === self.enemy ){

						if ( self.state === 'forward' && ( obj.state === 'wait' || obj.state === 'crouch' ) ){
							obj.animate.push( 3 );
							self.states.forward.easing[ 0 ] = 90;
							if ( obj.border ){
								self.animate.lock( obj.border );
							}
						}

						if ( self.currState === 'jump_forward_down' || self.currState === 'jumpDown' ){
							var distance = self.statusManage.get().enemy_distance;
							obj.animate.push( ( 234 - distance ) / 40 );
						}

						if ( self.state === 'forward' &&  obj.state === 'forward'  ){
							self.animate.lock( dir );	
						}

					}

				})


				this.keyManage.match( function( state ){

					if ( self.statusManage.isCrouch() && self.states[ 'crouch_' + state ] ){
						return 	self.play( 'crouch_' + state );
					}

					if ( !self.statusManage.isJump() && self.states[ 'near_' + state ] && self.statusManage.get().enemy_distance <= self.states[ 'near_' + state ].near  ){
						return self.play( 'near_' + state );
					}

					return self.play( state );
				});


				this.keyManage.unmatch( function( state ){

					if ( self.state === 'stand_up' && state === 'wait' ){
						return;
					}

					if ( self.state === 'back' || self.state === 'forward' || self.state === 'stand_up_defense' || self.state === 'stand_crouch_defense' || self.state === 'crouch' ){
						self.play( self.states.default );
					}

				});

			}


		}

	)



Spirit.interface( 'Animate', Interfaces.Animate );

Spirit.interface( 'SpiritFrames', Interfaces.SpiritFrames );
		
Spirit.interface( 'Shadow', Interfaces.Shadow );

Spirit.interface( 'Queue', Interfaces.Queue );

Spirit.interface( 'Lock', Interfaces.Lock );
		
Spirit.interface( 'KeyManage', Interfaces.KeyManage );
		
Spirit.interface( 'Collision', Interfaces.Collision );
		
Spirit.interface( 'StatusManage', Interfaces.StatusManage );



Fighter.interface( 'Animate', Interfaces.Animate );

Fighter.interface( 'Collision', Interfaces.Collision );

Fighter.interface( 'AttackEffect', Interfaces.AttackEffect );

Fighter.interface( 'Audio', Interfaces.Audio );




var WaveBoxing = Fighter.subClass( function( master ){

	this.width = 56;
	this.height = 32;
	this.direction = 1;
	this.master = master;
	this.easing = null;
	this.firing = false;
	this.ready_firing = false;
	this.timer = null;
	
	}, {
				init: function(){
					var self = this;
					this.frames = this.implement( 'SpiritFrames' );
					this.animate = this.implement( 'Animate' );
					this.collision = this.implement( 'Collision', 48, 32 );
					this.attackEffect = this.implement( 'AttackEffect', this );

					this.frames.event.listen( 'frameStart', function(){

						self.animate.move();

						self.firing && self.collision.check();
		
					})


					this.collision.event.listen( 'affirm',  function( obj, dir ){

						if ( obj === self.master.enemy.waveBoxing && self.firing && self.master.enemy.waveBoxing.firing ){
							self.stop();
							self.master.enemy.waveBoxing.stop();
							self.master.enemy.waveBoxing.attackEffect.start( self.easing[ 4 ], self.master.enemy.waveBoxing.direction === 1 ? self.left - self.width : self.left + self.width , self.top );
							return self.attackEffect.start( self.easing[ 4 ], self.left, self.top );
						}

						if ( obj === self.master.enemy ){

							if ( Math.abs( this.top - this.master.enemy.top ) > 130 ) return;  //��Ծ��ʱ�򽵵���ײ����

							var enemy_invincible = this.master.enemy.statusManage.get().invincible;

							if ( enemy_invincible ){
								return;
							}

							var enemy_attack_type = this.master.enemy.statusManage.get().attack_type;

							if ( enemy_attack_type === 'defense' ){
								setTimeout(function(){
									self.stop();
								}, 0)
								return self.enemyDefense();
							}

							setTimeout(function(){
								self.stop();
							}, 0)

							this.enemyBeat();

						}
						

	
					})

					this.frames.event.listen( 'framesDone', function(){
						self.frames.loop();
						self.animate.loop();
					})

					return this;
				},