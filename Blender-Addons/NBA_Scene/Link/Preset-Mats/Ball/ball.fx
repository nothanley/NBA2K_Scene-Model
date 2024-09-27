{
	"Version": 5,
	"WeightBits": 16,
	"Technique": {
				"Basketball": {
					"EnableMask": 2,
					"Pass": {
						"Final": {
							"EnableMask": 1,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.02350dd580b55534.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAeggAAgQIBAB7CgACBAgEAHwJAAIECAQAfQcAAgQIBAB-AAACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAOAAACDAgEAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAGAAACGAoEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEABMAAAIYCAQAFAAAAhgIBAAfAAACGBAEACAAAAIYEAQA"
						},
						"ProjectTexture": {
							"EnableMask": 2,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.c9d10afdc42d3e5c.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBAABBAIEAAIGAAEEAgQAAwMAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAA4AAAIMCAQABAAAABgaBAAGAAAAGBoEAAgAAAAYGgQACQAAABgaBAANAQABGAIEAAUAAAIYIgQABgAAAhgKBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAEgAAAhgKBAATAAACGAgEABQAAAIYCAQAGgAAAhgIBAAbAAACGAgEABwAAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						},
						"UseSoftShadowMaps": {
							"EnableMask": 4,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.02350dd580b55534.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAeggAAgQIBAB7CgACBAgEAHwJAAIECAQAfQcAAgQIBAB-AAACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAOAAACDAgEAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAGAAACGAoEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEABMAAAIYCAQAFAAAAhgIBAAfAAACGBAEACAAAAIYEAQA"
						},
						"UseHardShadowMaps": {
							"EnableMask": 8,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.64cd57925ad32327.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAAwIAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAA4AAAIMCAQABAAAABgaBAAGAAAAGBoEAAkAAAAYGgQADQEAARgCBAANAgABGAIEAAUAAAIYIgQABgAAAhgKBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAEgAAAhgKBAATAAACGAgEABQAAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						},
						"GBuffer": {
							"EnableMask": 512,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"INDEPENDENTBLENDENABLE": 1,
							"COLORWRITEENABLE4": 0,
							"COLORWRITEENABLE7": 0,
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R8G8B8A8_UNORM_SRGB"
									},
									"SV_TARGET1": {
										"Type": "uint4",
										"Format": "R8G8B8A8_UINT"
									},
									"SV_TARGET2": {
										"Type": "uint2",
										"Format": "R16G16_UINT"
									},
									"SV_TARGET3": {
										"Type": "uint2",
										"Format": "R32G32_UINT"
									},
									"SV_TARGET5": {
										"Type": "float3",
										"Format": "R11G11B10_FLOAT"
									},
									"SV_TARGET6": {
										"Type": "float2",
										"Format": "R16G16_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.b1c2b5113555eca4.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.99befd7a7685c078.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAHoIAAIECAQAewoAAgQIBAB8CQACBAgEAH0HAAIECAQAfgAAAgQIBAB_AQACBAgEAAoAAAAKWgAACwAAAApaBAAMAAAACloEAA0AAAIKYAAABAAAABhaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAIAAACGCIEAAwAAAIYFAQADQIAAhgIBAANEgACGAgEAA=="
						},
						"OnlyDepthDoubleSided": {
							"EnableMask": 32,
							"DepthFormat": "D16_UNORM",
							"CULLMODE": "NONE",
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"OnlyDepthCSM": {
							"EnableMask": 64,
							"DepthFormat": "D16_UNORM",
							"CULLMODE": "NONE",
							"CLIPPING": 0,
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"DepthOnlyPrepass": {
							"EnableMask": 128,
							"CULLMODE": "CW",
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"DepthNormalPrepass": {
							"EnableMask": 16,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "uint4",
										"Format": "R8G8B8A8_UINT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.cb4ecaaa25ad5ebf.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAYAAQQCBAB-CgACBAgEAH8JAAIECAQACgAAAApaAAALAAAACloAAAwAAAAKGgQADQAAAgpgAAAEAAAAGBoEAA0BAAEYAgQADQIAAhgIBAANEgACGAgEAA=="
						}
					}
				},
				"BasketballLores": {
					"EnableMask": 1,
					"Pass": {
						"Final": {
							"EnableMask": 1,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.02350dd580b55534.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAeggAAgQIBAB7CgACBAgEAHwJAAIECAQAfQcAAgQIBAB-AAACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAOAAACDAgEAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAGAAACGAoEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEABMAAAIYCAQAFAAAAhgIBAAfAAACGBAEACAAAAIYEAQA"
						},
						"ProjectTexture": {
							"EnableMask": 2,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.362352b2be31890b.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBAABBAIEAAIGAAEEAgQAAwMAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAA4AAAIMCAQABAAAABgaBAAGAAAAGBoEAAgAAAAYGgQACQAAABgaBAANAQABGAIEAAUAAAIYIgQABgAAAhgKBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAEgAAAhgKBAATAAACGAgEABQAAAIYCAQAGgAAAhgIBAAbAAACGAgEABwAAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						},
						"UseSoftShadowMaps": {
							"EnableMask": 4,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.02350dd580b55534.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAeggAAgQIBAB7CgACBAgEAHwJAAIECAQAfQcAAgQIBAB-AAACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAOAAACDAgEAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAGAAACGAoEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEABMAAAIYCAQAFAAAAhgIBAAfAAACGBAEACAAAAIYEAQA"
						},
						"UseHardShadowMaps": {
							"EnableMask": 8,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.d3b3252a46409d14.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAAwIAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAA4AAAIMCAQABAAAABgaBAAGAAAAGBoEAAkAAAAYGgQADQEAARgCBAANAgABGAIEAAUAAAIYIgQABgAAAhgKBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAEgAAAhgKBAATAAACGAgEABQAAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						},
						"GBuffer": {
							"EnableMask": 512,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"INDEPENDENTBLENDENABLE": 1,
							"COLORWRITEENABLE4": 0,
							"COLORWRITEENABLE7": 0,
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R8G8B8A8_UNORM_SRGB"
									},
									"SV_TARGET1": {
										"Type": "uint4",
										"Format": "R8G8B8A8_UINT"
									},
									"SV_TARGET2": {
										"Type": "uint2",
										"Format": "R16G16_UINT"
									},
									"SV_TARGET3": {
										"Type": "uint2",
										"Format": "R32G32_UINT"
									},
									"SV_TARGET5": {
										"Type": "float3",
										"Format": "R11G11B10_FLOAT"
									},
									"SV_TARGET6": {
										"Type": "float2",
										"Format": "R16G16_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.b1c2b5113555eca4.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.99befd7a7685c078.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAHoIAAIECAQAewoAAgQIBAB8CQACBAgEAH0HAAIECAQAfgAAAgQIBAB_AQACBAgEAAoAAAAKWgAACwAAAApaBAAMAAAACloEAA0AAAIKYAAABAAAABhaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAIAAACGCIEAAwAAAIYFAQADQIAAhgIBAANEgACGAgEAA=="
						},
						"OnlyDepthDoubleSided": {
							"EnableMask": 32,
							"DepthFormat": "D16_UNORM",
							"CULLMODE": "NONE",
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"OnlyDepthCSM": {
							"EnableMask": 64,
							"DepthFormat": "D16_UNORM",
							"CULLMODE": "NONE",
							"CLIPPING": 0,
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"DepthOnlyPrepass": {
							"EnableMask": 128,
							"CULLMODE": "CW",
							"COLORWRITEENABLE": 0,
							"MASTERCOLORWRITEENABLE": 0,
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.c40135385936ba71.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloAAAwAAAAKWgAADQAAAgpgAAA="
						},
						"DepthNormalPrepass": {
							"EnableMask": 16,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"STENCILENABLE": 1,
							"STENCILFUNC": "ALWAYS",
							"STENCILPASS": "REPLACE",
							"STENCILMASK": 128,
							"STENCILWRITEMASK": 63,
							"STENCILREF": 32,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "uint4",
										"Format": "R8G8B8A8_UINT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.cb4ecaaa25ad5ebf.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAYAAQQCBAB-CgACBAgEAH8JAAIECAQACgAAAApaAAALAAAACloAAAwAAAAKGgQADQAAAgpgAAAEAAAAGBoEAA0BAAEYAgQADQIAAhgIBAANEgACGAgEAA=="
						}
					}
				},
				"BasketballVelocity": {
					"EnableMask": 1024,
					"Pass": {
						"Default": {
							"EnableMask": 15,
							"CULLMODE": "CW",
							"ZWRITEENABLE": 1,
							"PS": {
								"Input": {
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float2",
										"Format": "R16G16_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.94eaa70666002a4c.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float3",
										"BindPoint": 0
									}
								},
								"Output": {
									"TEXCOORD11": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.46556cc24757a051.shader"
							},
							"ResourceMapping": "CgAAAApaAAALAAAACloEAAwAAAAKWgAADQAAAgpgAAAEAAAAGFoEAA=="
						}
					}
				},
				"BallMirror": {
					"EnableMask": 8,
					"Pass": {
						"UseHardShadowMaps": {
							"EnableMask": 8,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT_S8X24_UINT",
							"CULLMODE": "CW",
							"FRONT_CCW": 1,
							"STENCILENABLE": 1,
							"STENCILFUNC": "EQUAL",
							"STENCILPASS": "REPLACE",
							"STENCILWRITEMASK": 191,
							"STENCILMASK": 64,
							"STENCILREF": 96,
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.728f553acbdd6d7d.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									},
									"SV_CLIPDISTANCE0": {
										"Type": "float"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.edf235105dc7c786.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAAwIAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQADQIAARgCBAAFAAACGCIEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEAB8AAAIYEAQAIAAAAhgQBAA="
						}
					}
				},
				"Reflection": {
					"EnableMask": 256,
					"Pass": {
						"Default": {
							"EnableMask": 1,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT",
							"CULLMODE": "CCW",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.801bde84f9020f3c.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.1f3bc98160942ed3.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAH0IAAIECAQAfgcAAgQIBAB_AQACBAgEAAoAAAAKWgAACwAAAApaBAAMAAAAChoEAA0AAAIKYAAABAAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						},
						"ProjectTexture": {
							"EnableMask": 2,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT",
							"CULLMODE": "CCW",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.f8f6e11a78fabf76.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.1f3bc98160942ed3.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBAABBAIEAAIGAAEEAgQAfQgAAgQIBAB-BwACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAEAAAAGBoEAAgAAAAYGgQACQAAABgaBAANAQABGAIEAAUAAAIYIgQACAAAAhgiBAAJAAACGCIEAAwAAAIYFAQADQIAAhgIBAANEgACGAgEABoAAAIYCAQAGwAAAhgIBAAcAAACGAgEAB8AAAIYEAQAIAAAAhgQBAA="
						},
						"UseShadowMapPass": {
							"EnableMask": 12,
							"BakePso": 1,
							"DepthFormat": "D32_FLOAT",
							"CULLMODE": "CCW",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.96bb210400210654.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "float4"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.1f3bc98160942ed3.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAICAAEEAgQAfQgAAgQIBAB-BwACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAEAAAAGBoEAAYAAAAYGgQACQAAABgaBAANAQABGAIEAA0CAAEYAgQABQAAAhgiBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAHwAAAhgQBAAgAAACGBAEAA=="
						}
					}
				},
				"BasketReflection": {
					"EnableMask": 2048,
					"Pass": {
						"Default": {
							"EnableMask": 1,
							"ALPHABLENDENABLE": 1,
							"BLENDOP": "ADD",
							"SRCBLEND": "SRCALPHA",
							"DESTBLEND": "INVSRCALPHA",
							"ZFUNC": "ALWAYS",
							"ZWRITEENABLE": 0,
							"CULLMODE": "CCW",
							"STENCILENABLE": 1,
							"STENCILMASK": 8,
							"STENCILREF": 8,
							"STENCILFUNC": "EQUAL",
							"STENCILPASS": "KEEP",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.9ffce5ec1c63dbcd.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAeggAAgQIBAB7CgACBAgEAHwJAAIECAQAfQcAAgQIBAB-AAACBAgEAH8BAAIECAQACgAAAApaAAALAAAACloEAAwAAAAKGgQADQAAAgpgAAAEAAAAGBoEAAkAAAAYGgQADQEAARgCBAAFAAACGCIEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEAB8AAAIYEAQAIAAAAhgQBAA="
						},
						"ProjectTexture": {
							"EnableMask": 2,
							"ALPHABLENDENABLE": 1,
							"BLENDOP": "ADD",
							"SRCBLEND": "SRCALPHA",
							"DESTBLEND": "INVSRCALPHA",
							"ZFUNC": "ALWAYS",
							"ZWRITEENABLE": 0,
							"CULLMODE": "CCW",
							"STENCILENABLE": 1,
							"STENCILMASK": 8,
							"STENCILREF": 8,
							"STENCILFUNC": "EQUAL",
							"STENCILPASS": "KEEP",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.75b96cabc50705fb.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBAABBAIEAAIGAAEEAgQAAwMAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAAQAAAAYGgQACAAAABgaBAAJAAAAGBoEAA0BAAEYAgQABQAAAhgiBAAIAAACGCIEAAkAAAIYIgQADAAAAhgUBAANAgACGAgEAA0SAAIYCAQAEgAAAhgKBAAaAAACGAgEABsAAAIYCAQAHAAAAhgIBAAfAAACGBAEACAAAAIYEAQA"
						},
						"UseShadowMaps": {
							"EnableMask": 12,
							"ALPHABLENDENABLE": 1,
							"BLENDOP": "ADD",
							"SRCBLEND": "SRCALPHA",
							"DESTBLEND": "INVSRCALPHA",
							"ZFUNC": "ALWAYS",
							"ZWRITEENABLE": 0,
							"CULLMODE": "CCW",
							"STENCILENABLE": 1,
							"STENCILMASK": 8,
							"STENCILREF": 8,
							"STENCILFUNC": "EQUAL",
							"STENCILPASS": "KEEP",
							"PS": {
								"Input": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Output": {
									"SV_TARGET0": {
										"Type": "float4",
										"Format": "R11G11B10_FLOAT"
									}
								},
								"Profile": "ps_6_0",
								"Binary": "PS.9e52cc93addb31c4.shader"
							},
							"VS": {
								"Input": {
									"POSITION0": {
										"Type": "float4",
										"BindPoint": 0
									},
									"TANGENTFRAME0": {
										"Type": "uint4",
										"BindPoint": 1
									},
									"TEXCOORD0": {
										"Type": "float2",
										"BindPoint": 2
									},
									"SV_INSTANCEID0": {
										"Type": "uint"
									}
								},
								"Output": {
									"TEXCOORD0": {
										"Type": "uint2"
									},
									"TEXCOORD1": {
										"Type": "float4"
									},
									"TEXCOORD2": {
										"Type": "float4"
									},
									"TEXCOORD3": {
										"Type": "float4"
									},
									"SV_POSITION0": {
										"Type": "float4"
									}
								},
								"Profile": "vs_6_0",
								"Binary": "VS.9a4f599ae0db2492.shader"
							},
							"ResourceMapping": "AAUAAQQCBAABBgABBAIEAAIDAAEEAgQAAwIAAQQCBAB6CAACBAgEAHsKAAIECAQAfAkAAgQIBAB9BwACBAgEAH4AAAIECAQAfwEAAgQIBAAKAAAACloAAAsAAAAKWgQADAAAAAoaBAANAAACCmAAAAQAAAAYGgQABgAAABgaBAAJAAAAGBoEAA0BAAEYAgQADQIAARgCBAAFAAACGCIEAAgAAAIYIgQACQAAAhgiBAAMAAACGBQEAA0CAAIYCAQADRIAAhgIBAASAAACGAoEAB8AAAIYEAQAIAAAAhgQBAA="
						}
					}
				}
	},
	"Parameter": {
				"AlbedoIntensity": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 1.0,
					"Offset": 352
				},
				"BasketGlassZPos": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 1310.64,
					"Offset": 364
				},
				"BasketInvMaxReflectorDistance": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 0.0131233596,
					"Offset": 356
				},
				"BasketReflectionIntensity": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 0.4,
					"Offset": 292
				},
				"BlackLightEmitIntensity": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 2.0,
					"Offset": 360
				},
				"BLAlbedoTexture_DEFAULT": {
					"Type": "float4",
					"NonWidget": 1,
					"Offset": 128
				},
				"BLAlbedoTexture_ENABLE": {
					"Type": "bool",
					"Usage": "BLAlbedoTexture_ENABLE",
					"Offset": 296
				},
				"BLAlbedoTexture_OFFSET": {
					"Type": "float4",
					"Usage": "BLAlbedoTexture_OFFSET",
					"Offset": 176
				},
				"BLAlbedoTexture_SCALE": {
					"Type": "float4",
					"Usage": "BLAlbedoTexture_SCALE",
					"Offset": 192
				},
				"BLAlbedoTexture_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 232
				},
				"ColorizeRefraction": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 1.0,
					"Offset": 260
				},
				"ColorTexture_OFFSET": {
					"Type": "float4",
					"Usage": "ColorTexture_OFFSET",
					"Offset": 160
				},
				"ColorTexture_SCALE": {
					"Type": "float4",
					"Usage": "ColorTexture_SCALE",
					"Offset": 144
				},
				"ColorTexture_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 224
				},
				"ConstantIsolate": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 1.0,
					"Offset": 300
				},
				"ConstantIsolatePCF": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 1.0,
					"Offset": 316
				},
				"EmissiveIntensity": {
					"Type": "float",
					"NonWidget": 1,
					"Offset": 336
				},
				"EmissiveTexture_DEFAULT": {
					"Type": "float4",
					"NonWidget": 1,
					"Offset": 64
				},
				"EmissiveTexture_ENABLE": {
					"Type": "bool",
					"Usage": "EmissiveTexture_ENABLE",
					"Offset": 272
				},
				"EmissiveTexture_OFFSET": {
					"Type": "float4",
					"Usage": "EmissiveTexture_OFFSET",
					"Offset": 80
				},
				"EmissiveTexture_SCALE": {
					"Type": "float4",
					"Usage": "EmissiveTexture_SCALE",
					"Offset": 96
				},
				"EmissiveTexture_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 240
				},
				"EnableRefraction": {
					"Type": "bool",
					"Offset": 288
				},
				"BLAlbedoTexture": {
					"Type": "Texture2D",
					"Offset": 264
				},
				"ColorTexture": {
					"Type": "Texture2D",
					"Offset": 284
				},
				"ConstSamplerComparisonState_b78e7445": {
					"Type": "SamplerComparisonState",
					"Offset": 340
				},
				"ConstSamplerState_390173a6": {
					"Type": "SamplerState",
					"Offset": 320
				},
				"ConstSamplerState_8562905f": {
					"Type": "SamplerState",
					"Offset": 304
				},
				"ConstSamplerState_b78e7445": {
					"Type": "SamplerState",
					"Offset": 368
				},
				"ConstSamplerState_fca957e1": {
					"Type": "SamplerState",
					"Offset": 324
				},
				"EmissiveTexture": {
					"Type": "Texture2D",
					"Offset": 256
				},
				"IridescencePropertyMap": {
					"Type": "Texture2D<half4>",
					"Offset": 308
				},
				"NormalTexture": {
					"Type": "Texture2D",
					"Offset": 268
				},
				"RoughnessDensityMetalnessMap": {
					"Type": "Texture2D<half4>",
					"Offset": 348
				},
				"IridescenceOverride": {
					"Type": "bool",
					"Offset": 280
				},
				"IridescencePropertyMap_ENABLE": {
					"Type": "bool",
					"Usage": "IridescencePropertyMap_ENABLE",
					"Offset": 328
				},
				"IridescencePropertyMap_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 248
				},
				"LastObjectToWorld": {
					"Type": "float4x4",
					"NonWidget": 1,
					"Offset": 0
				},
				"Lighting_Disable_SS_Effects": {
					"Type": "bool",
					"NonWidget": 1,
					"Offset": 332
				},
				"NormalTexture_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 216
				},
				"OneOverIoR": {
					"Type": "float",
					"NonWidget": 1,
					"Offset": 312
				},
				"RefractionRoughness": {
					"Type": "float",
					"NonWidget": 1,
					"Value": 0.05,
					"Offset": 276
				},
				"RoughnessDensityMetalnessMap_DEFAULT": {
					"Type": "float4",
					"NonWidget": 1,
					"Offset": 112
				},
				"RoughnessDensityMetalnessMap_ENABLE": {
					"Type": "bool",
					"Usage": "RoughnessDensityMetalnessMap_ENABLE",
					"Offset": 344
				},
				"RoughnessDensityMetalnessMap_TexcoordScaleXY": {
					"Type": "float2",
					"NonWidget": 1,
					"Value": [ 1.0, 1.0 ],
					"Offset": 208
				}
	},
	"Resource": {
				"BLAlbedoTexture": {
					"NonWidget": 1,
					"TexcoordScale": "BLAlbedoTexture_TexcoordScaleXY",
					"Texcoord": 0
				},
				"ColorTexture": {
					"NonWidget": 1,
					"TexcoordScale": "ColorTexture_TexcoordScaleXY",
					"Texcoord": 0
				},
				"ConstSamplerComparisonState_b78e7445": {
					"Type": "SAMPLERSTATE",
					"NonWidget": 1,
					"COMPAREFUNC": "GREATER",
					"ADDRESSU": "CLAMP",
					"ADDRESSV": "CLAMP",
					"MINFILTER": "LINEAR",
					"MAGFILTER": "LINEAR"
				},
				"ConstSamplerState_390173a6": {
					"Type": "SAMPLERSTATE",
					"NonWidget": 1,
					"ADDRESSU": "CLAMP",
					"ADDRESSV": "CLAMP",
					"ADDRESSW": "CLAMP",
					"MINFILTER": "LINEAR",
					"MAGFILTER": "LINEAR",
					"MIPFILTER": "POINT",
					"MAXMIPLEVEL": 0
				},
				"ConstSamplerState_8562905f": {
					"Type": "SAMPLERSTATE",
					"NonWidget": 1,
					"ADDRESSU": "BORDER",
					"ADDRESSV": "BORDER",
					"MINFILTER": "LINEAR",
					"MAGFILTER": "LINEAR",
					"MIPFILTER": "LINEAR",
					"BORDERCOLOR": [ 0.0, 0.0, 0.0, 0.0 ]
				},
				"ConstSamplerState_b78e7445": {
					"Type": "SAMPLERSTATE",
					"NonWidget": 1,
					"ADDRESSU": "CLAMP",
					"ADDRESSV": "CLAMP",
					"MINFILTER": "LINEAR",
					"MAGFILTER": "LINEAR",
					"MIPFILTER": "LINEAR"
				},
				"ConstSamplerState_fca957e1": {
					"Type": "SAMPLERSTATE",
					"NonWidget": 1,
					"ADDRESSU": "WRAP",
					"ADDRESSV": "WRAP",
					"ADDRESSW": "CLAMP",
					"MINFILTER": "ANISOTROPIC",
					"MAXANISOTROPY": 4,
					"MAGFILTER": "ANISOTROPIC",
					"MIPFILTER": "LINEAR"
				},
				"EmissiveTexture": {
					"NonWidget": 1,
					"TexcoordScale": "EmissiveTexture_TexcoordScaleXY",
					"Texcoord": 0
				},
				"IridescencePropertyMap": {
					"NonWidget": 1,
					"Result": "half4",
					"TexcoordScale": "IridescencePropertyMap_TexcoordScaleXY",
					"Texcoord": 0
				},
				"NormalTexture": {
					"NonWidget": 1,
					"TexcoordScale": "NormalTexture_TexcoordScaleXY",
					"Texcoord": 0
				},
				"RoughnessDensityMetalnessMap": {
					"NonWidget": 1,
					"Result": "half4",
					"TexcoordScale": "RoughnessDensityMetalnessMap_TexcoordScaleXY",
					"Texcoord": 0
				}
	}
}

