{
	'includes':
	[
		'../../../common.gypi',
	],
	
	'targets':
	[
		{
			'target_name': 'reflex',
			'type': 'executable',
			
			'toolsets': ['host','target'],
			
			'product_name': 'reflex-<(_toolset)',
		
			'variables':
			{
				'silence_warnings': 1,
			},
	
			'direct_dependent_settings':
			{
				'variables':
				{
					'reflex_exe_file': '<(PRODUCT_DIR)/<(_product_name)<(EXECUTABLE_SUFFIX)',
				},
			},
			
			'sources':
			[
				'reflex.c',
			],
			
			# Gentle does not work on 64-bit Windows
			#'msvs_target_platform': 'x86',

			'msvs_settings':
			{
				'VCLinkerTool':
				{
					'SubSystem': '1',	# /SUBSYSTEM:CONSOLE
				},
			},
		},
	],
}

