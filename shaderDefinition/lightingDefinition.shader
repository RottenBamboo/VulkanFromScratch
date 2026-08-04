{
    "name": "DefaultLit",
    "stages": [
        {
            "stage": "vertex",
            "path": "shader/bin/lightingVert.spv"
        },
        {
            "stage": "fragment",
            "path": "shader/bin/lightingFrag.spv"
        }
    ],
    "parameters": [
        {
            "name": "gPosition",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 1
        },
        {
            "name": "gNormal",
            "type": "texture2D",
            "textureType": "Normal",
            "set": 1,
            "binding": 2
        },
        {
            "name": "gAlbedo",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 3
        },
	 {
            "name": "gMetallicRoughnessAO",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 4
        }
    ]
}