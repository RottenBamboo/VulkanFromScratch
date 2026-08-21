{
    "name": "DefaultLit",
    "stages": [
        {
            "stage": "vertex",
            "path": "shader/bin/gBufferVert.spv"
        },
        {
            "stage": "fragment",
            "path": "shader/bin/gBufferFrag.spv"
        }
    ],
    "parameters": [
        {
            "name": "albedoMap",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 1
        },
        {
            "name": "normalMap",
            "type": "texture2D",
            "textureType": "Normal",
            "set": 1,
            "binding": 2
        },
        {
            "name": "metallicRoughnessAOMap",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 3
        },
	 {
            "name": "emissionMap",
            "type": "texture2D",
	    "textureType": "Albedo",
            "set": 1,
            "binding": 4
        }
    ]
}