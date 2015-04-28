{
	'includes':
	[
		'../common.gypi',
	],
	
	'variables':
	{
		'revzip_sources':
		[
			'src/revzip.cpp',
		],
	},
	
	'targets':
	[
		{
			'target_name': 'revzip',
			'type': 'loadable_module',
			'mac_bundle': 1,
			'product_prefix': '',
			'product_name': 'revzip',
			
			'dependencies':
			[
				'../libexternal/libexternal.gyp:libExternal',
				'../thirdparty/libzip/libzip.gyp:libzip',
			],
			
			'sources':
			[
				'<@(revzip_sources)',
			],
			
			'xcode_settings':
			{
				'INFOPLIST_FILE': 'rsrc/revzip-info.plist',
			},
			
			'conditions':
			[
				[
					'OS == "mac"',
					{
						'libraries':
						[
							'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
						],
					},
				],
				[
					'OS == "android"',
					{
						'product_name': 'RevZip',
						'product_extension': '',
					},
				],
			],
			
			'variables':
			{
				'ios_external_symbols': [ '_getXtable' ],
			},
			
			'all_dependent_settings':
			{
				'variables':
				{
					'dist_files': [ '<(PRODUCT_DIR)/<(_product_name)>(ext_bundle_suffix)' ],
				},
			},
		},
		{
			'target_name': 'revzip-server',
			'type': 'loadable_module',
			'product_prefix': '',
			'product_name': 'server-revzip',
			
			'dependencies':
			[
				'../libexternal/libexternal.gyp:libExternal',
				'../thirdparty/libzip/libzip.gyp:libzip',
			],
			
			'sources':
			[
				'<@(revzip_sources)',
			],
			
			'xcode_settings':
			{
				'INFOPLIST_FILE': 'rsrc/revzip-info.plist',
			},
			
			'conditions':
			[
				[
					'OS == "mac"',
					{						
						'libraries':
						[
							'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
						],
					},
				],
			],
			
			'variables':
			{
				'ios_external_symbols': [ '_getXtable' ],
			},
			
			'all_dependent_settings':
			{
				'variables':
				{
					'dist_files': [ '<(PRODUCT_DIR)/<(_product_name)>(lib_suffix)' ],
				},
			},
		},
	],
}
