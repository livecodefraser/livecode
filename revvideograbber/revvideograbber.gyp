{
	'includes':
	[
		'../common.gypi',
	],
	
	'targets':
	[
		{
			'target_name': 'external-revvideograbber',
			'type': 'loadable_module',
			'mac_bundle': 1,
			'product_prefix': '',
			'product_name': 'revvideograbber',

			'variables':
			{
				'enable_revvideograbber%': '1',
			},
			
			'dependencies':
			[
				'../libcore/libcore.gyp:libCore',
				'../libexternal/libexternal.gyp:libExternal',
			],
			
			'include_dirs':
			[
				'src',
			],
			
			'sources':
			[
				'src/dsvideograbber.h',
				'src/mcivideograbber.h',
				'src/qedit.h',
				'src/revcapture.h',
				'src/revvideograbber.h',
				'src/videograbber.h',
				
				'src/dsDlgFunc.cpp',
				'src/dsvideograbber.cpp',
				'src/mcivideograbber.cpp',
				'src/revcapture.mm',
				'src/revvideograbber.cpp',
				'src/rrecapture.m',
			],
			
			'conditions':
			[
				[
					'OS != "win"',
					{
						'type': 'none',
						'mac_bundle': '0',
					},
					{
						'all_dependent_settings':
						{
							'variables':
							{
								'dist_files': [ '<(PRODUCT_DIR)/<(_product_name)>(ext_bundle_suffix)' ],
							},
						},
					},
				],
				[
					'OS == "win" and enable_revvideograbber != 0',
					{
						'libraries':
						[
							'-lgdi32',
							'-luser32',
							'-ladvapi32',
							'-lole32',
							'-loleaut32',
							'-lstrmiids',
							'-lvfw32',
						],
					},
				],
			],
		},
	],
}
