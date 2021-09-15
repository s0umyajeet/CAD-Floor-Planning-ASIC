{
		if (GraphicEngine::firstParse) {
			static float f = 0.0f;
			static int row_count = GraphicEngine::row_count;

			ImGui::Begin("Editor Window");

			static ImGuiTableFlags flags = ImGuiTableFlags_Resizable |
				ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
				ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

			static std::vector <std::string> cmdVec;
			static std::vector <std::string> shapeVec;
			static std::vector <std::string> refVec;
			static std::vector <int>	 offsetXVec;
			static std::vector <int>	 offsetYVec;


			if (ImGui::BeginTable("table1", 5, flags))
			{
				ImGui::TableSetupColumn("cmd");
				ImGui::TableSetupColumn("Shape Name");
				ImGui::TableSetupColumn("Reference Shape");
				ImGui::TableSetupColumn("Pos X");
				ImGui::TableSetupColumn("Pos Y");
				ImGui::TableHeadersRow();

				if (run == 0) {
					//populate table fields with existing inputs

					//cmd
					for (auto i = 0; i < row_count; i++) cmdVec.push_back("place");

					//shape
					for (auto i = 0; i < row_count; i++) shapeVec.push_back(_current_draw_list[i]);

					//ref
					for (auto i = 0; i < row_count; i++) {
						auto shape = _current_draw_list[i];
						auto x = ActiveShapeBuffer::get().shapePlacementMap[shape];
						refVec.push_back(x.get_ref_id());
					}

					//offsetX
					for (auto i = 0; i < row_count; i++) {
						auto shape = _current_draw_list[i];
						auto x = ActiveShapeBuffer::get().shapePlacementMap[shape];
						offsetXVec.push_back(x.getoffsetX());
					}

					//offsetY
					for (auto i = 0; i < row_count; i++) {
						auto shape = _current_draw_list[i];
						auto x = ActiveShapeBuffer::get().shapePlacementMap[shape];
						offsetYVec.push_back(x.getoffsetY());
					}

				}

				int pos_x = 0, pos_y = 0;


				rapidcsv::Document doc = ActiveShapeBuffer::get().placementFile;

				if (run > 0) {
					for (int row = 0; row < row_count; row++)
					{
						ImGui::TableNextRow();
						for (int column = 0; column < 5; column++)
						{
							ImGui::TableSetColumnIndex(column);
							switch (column) {
							case 0:
								ImGui::PushID(column + row * 10);
								ImGui::InputText("##label", &cmdVec[row]);
								ImGui::PopID();
								break;
							case 1:
								ImGui::PushID(column + row * 10);
								ImGui::InputText("##label", &shapeVec[row]);
								ImGui::PopID();
								break;
							case 2:
								ImGui::PushID(column + row * 10);
								ImGui::InputText("##label", &refVec[row]);
								ImGui::PopID();
								break;
							case 3:
								ImGui::PushID(column + row * 10);
								ImGui::InputInt("##label", &offsetXVec[row]);
								ImGui::PopID();
								break;
							case 4:
								ImGui::PushID(column + row * 10);
								ImGui::InputInt("##label", &offsetYVec[row]);
								ImGui::PopID();
								break;
							}
						}


						doc.SetRow<std::string>(
							row, std::vector<std::string> {cmdVec[row], shapeVec[row], refVec[row],
							std::to_string(offsetXVec[row]), std::to_string(offsetYVec[row]) });

						doc.Save();

						//std::cout << "current draw list size: " << _current_draw_list.size() << std::endl;
						_current_draw_list.resize(0);
					}

				}

				ImGui::EndTable();
				/*
				if (ImGui::Button("Add 5 more")) {
					row_count += 5;
					cmdVec.resize(cmdVec.size() + 5);
					shapeVec.resize(shapeVec.size() + 5);
					refVec.resize(refVec.size() + 5);
					offsetXVec.resize(offsetXVec.size() + 5);
					offsetYVec.resize(offsetYVec.size() + 5);
				}
				*/

			}