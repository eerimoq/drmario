all:
	make_image_classifier \
	  --image_dir images/cells \
	  --tfhub_module https://tfhub.dev/google/tf2-preview/mobilenet_v2/feature_vector/4 \
	  --image_size 224 \
	  --saved_model_dir model \
	  --labels_output_file labels.txt \
	  --tflite_output_file model.tflite \
	  --summaries_dir logs
