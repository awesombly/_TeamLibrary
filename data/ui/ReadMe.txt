	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/test_1.txt", *pUIRoot);
	pUIRoot->find_child(L"panel1")->m_bRender = false;
	pUIRoot->find_child(L"txt_Guest")->EventClick.first = UI::E_INTRO_SHOW_GUEST;
	pUIRoot->find_child(L"txt_Guest")->EventClick.second = pUIRoot->find_child(L"panel1");

	pUIRoot->find_child(L"Guest_Exit")->EventClick.first = UI::E_INTRO_NOTSHOW_GUEST;
	pUIRoot->find_child(L"Guest_Exit")->EventClick.second = pUIRoot->find_child(L"panel1");