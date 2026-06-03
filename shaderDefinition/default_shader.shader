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
            "set": 1,
            "binding": 1
        },
        {
            "name": "normalMap",
            "type": "texture2D",
            "set": 1,
            "binding": 2
        },
        {
            "name": "metallicRoughnessAOMap",
            "type": "texture2D",
            "set": 1,
            "binding": 3
        },
	 {
            "name": "metallicRoughnessAOMap",
            "type": "texture2D",
            "set": 1,
            "binding": 4
        }
    ]
}