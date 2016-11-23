{
	'includes':
	[
		'../../../common.gypi',
	],
	
	'targets':
	[
		{
			'target_name': 'grts',
			'type': 'static_library',
			
			'toolsets': ['host','target'],
			
			'product_name': 'grts',
		
			'variables':
			{
				'silence_warnings': 1,
			},
	
			'sources':
			[
				'grts.c',
			],
			
			# Gentle does not work on 64-bit Windows
			#'msvs_target_platform': 'x86',

			'target_conditions':
			[
				[
					'_toolset != "target"',
					{
						'product_name': 'grts->(_toolset)',
					},
				],
			],
		},
	],
}
