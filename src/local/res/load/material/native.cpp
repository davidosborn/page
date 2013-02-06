/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include <memory> // {shared,unique}_ptr

#include "../../../util/lexical_cast.hpp"
#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/extract.hpp" // Extract
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/material.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../Stream.hpp" // Stream::~Stream
#include "../../type/Material.hpp"
#include "../register.hpp" // LoadFunction, REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnPass(Material &mat, const std::string &)
			{
				mat.passes.push_back(Material::Pass());
			}
			void OnPassAmbientColor(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().ambient.color);
			}
			void OnPassAmbientTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().ambient.texture.image = cache::Resource<Image>(value);
			}
			void OnPassAmbientTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().ambient.texture.image = cache::Resource<Image>(value);
			}
			void OnPassAmbientTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().ambient.texture.offset);
			}
			void OnPassAmbientTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().ambient.texture.scale);
			}
			void OnPassAmbientTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().ambient.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassDiffuseColor(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().diffuse.color);
			}
			void OnPassDiffuseTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().diffuse.texture.image = cache::Resource<Image>(value);
			}
			void OnPassDiffuseTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().diffuse.texture.image = cache::Resource<Image>(value);
			}
			void OnPassDiffuseTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().diffuse.texture.offset);
			}
			void OnPassDiffuseTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().diffuse.texture.scale);
			}
			void OnPassDiffuseTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().diffuse.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassEmissiveColor(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().emissive.color);
			}
			void OnPassEmissiveTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().emissive.texture.image = cache::Resource<Image>(value);
			}
			void OnPassEmissiveTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().emissive.texture.image = cache::Resource<Image>(value);
			}
			void OnPassEmissiveTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().emissive.texture.offset);
			}
			void OnPassEmissiveTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().emissive.texture.scale);
			}
			void OnPassEmissiveTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().emissive.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassFresnelColor(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().fresnel.color);
			}
			void OnPassFresnelPowerTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().fresnel.power.texture.image = cache::Resource<Image>(value);
			}
			void OnPassFresnelPowerTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().fresnel.power.texture.image = cache::Resource<Image>(value);
			}
			void OnPassFresnelPowerTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().fresnel.power.texture.offset);
			}
			void OnPassFresnelPowerTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().fresnel.power.texture.scale);
			}
			void OnPassFresnelPowerTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().fresnel.power.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassFresnelPowerValue(Material &mat, const std::string &value)
			{
				mat.passes.back().fresnel.power.value = util::lexical_cast<float>(value);
			}
			void OnPassFresnelTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().fresnel.texture.image = cache::Resource<Image>(value);
			}
			void OnPassFresnelTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().fresnel.texture.image = cache::Resource<Image>(value);
			}
			void OnPassFresnelTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().fresnel.texture.offset);
			}
			void OnPassFresnelTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().fresnel.texture.scale);
			}
			void OnPassFresnelTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().fresnel.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassGlossTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().gloss.texture.image = cache::Resource<Image>(value);
			}
			void OnPassGlossTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().gloss.texture.image = cache::Resource<Image>(value);
			}
			void OnPassGlossTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().gloss.texture.offset);
			}
			void OnPassGlossTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().gloss.texture.scale);
			}
			void OnPassGlossTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().gloss.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassGlossValue(Material &mat, const std::string &value)
			{
				mat.passes.back().gloss.value = util::lexical_cast<float>(value);
			}
			void OnPassMaskTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().mask.texture.image = cache::Resource<Image>(value);
			}
			void OnPassMaskTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().mask.texture.image = cache::Resource<Image>(value);
			}
			void OnPassMaskTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().mask.texture.offset);
			}
			void OnPassMaskTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().mask.texture.scale);
			}
			void OnPassMaskTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().mask.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassMaskValue(Material &mat, const std::string &value)
			{
				mat.passes.back().mask.value = util::lexical_cast<float>(value);
			}
			void OnPassNormalTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().normal.texture.image = cache::Resource<Image>(value);
			}
			void OnPassNormalTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().normal.texture.image = cache::Resource<Image>(value);
			}
			void OnPassNormalTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().normal.texture.offset);
			}
			void OnPassNormalTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().normal.texture.scale);
			}
			void OnPassNormalTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().normal.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassSpecularColor(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().specular.color);
			}
			void OnPassSpecularPowerTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().specular.power.texture.image = cache::Resource<Image>(value);
			}
			void OnPassSpecularPowerTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().specular.power.texture.image = cache::Resource<Image>(value);
			}
			void OnPassSpecularPowerTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().specular.power.texture.offset);
			}
			void OnPassSpecularPowerTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().specular.power.texture.scale);
			}
			void OnPassSpecularPowerTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().specular.power.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}
			void OnPassSpecularPowerValue(Material &mat, const std::string &value)
			{
				mat.passes.back().specular.power.value = util::lexical_cast<float>(value);
			}
			void OnPassSpecularTexture(Material &mat, const std::string &value)
			{
				if (!value.empty()) mat.passes.back().specular.texture.image = cache::Resource<Image>(value);
			}
			void OnPassSpecularTextureImage(Material &mat, const std::string &value)
			{
				mat.passes.back().specular.texture.image = cache::Resource<Image>(value);
			}
			void OnPassSpecularTextureOffset(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().specular.texture.offset);
			}
			void OnPassSpecularTextureScale(Material &mat, const std::string &value)
			{
				Extract(value, mat.passes.back().specular.texture.scale);
			}
			void OnPassSpecularTextureUv(Material &mat, const std::string &value)
			{
				mat.passes.back().specular.texture.uvIndex = util::lexical_cast<unsigned>(value);
			}

			// parser factory
			inline Parser GetParser(Material &mat)
			{
				Parser parser;
				parser.Register("pass",                               Bind(OnPass,                           mat), Parser::noValue);
				parser.Register("pass.ambient");
				parser.Register("pass.ambient.color",                 Bind(OnPassAmbientColor,               mat), Parser::needValue);
				parser.Register("pass.ambient.texture",               Bind(OnPassAmbientTexture,             mat));
				parser.Register("pass.ambient.texture.image",         Bind(OnPassAmbientTextureImage,        mat), Parser::needValue);
				parser.Register("pass.ambient.texture.offset",        Bind(OnPassAmbientTextureOffset,       mat), Parser::needValue);
				parser.Register("pass.ambient.texture.scale",         Bind(OnPassAmbientTextureScale,        mat), Parser::needValue);
				parser.Register("pass.ambient.texture.uv",            Bind(OnPassAmbientTextureUv,           mat), Parser::needValue);
				parser.Register("pass.diffuse");
				parser.Register("pass.diffuse.color",                 Bind(OnPassDiffuseColor,               mat), Parser::needValue);
				parser.Register("pass.diffuse.texture",               Bind(OnPassDiffuseTexture,             mat));
				parser.Register("pass.diffuse.texture.image",         Bind(OnPassDiffuseTextureImage,        mat), Parser::needValue);
				parser.Register("pass.diffuse.texture.offset",        Bind(OnPassDiffuseTextureOffset,       mat), Parser::needValue);
				parser.Register("pass.diffuse.texture.scale",         Bind(OnPassDiffuseTextureScale,        mat), Parser::needValue);
				parser.Register("pass.diffuse.texture.uv",            Bind(OnPassDiffuseTextureUv,           mat), Parser::needValue);
				parser.Register("pass.emissive");
				parser.Register("pass.emissive.color",                Bind(OnPassEmissiveColor,              mat), Parser::needValue);
				parser.Register("pass.emissive.texture",              Bind(OnPassEmissiveTexture,            mat));
				parser.Register("pass.emissive.texture.image",        Bind(OnPassEmissiveTextureImage,       mat), Parser::needValue);
				parser.Register("pass.emissive.texture.offset",       Bind(OnPassEmissiveTextureOffset,      mat), Parser::needValue);
				parser.Register("pass.emissive.texture.scale",        Bind(OnPassEmissiveTextureScale,       mat), Parser::needValue);
				parser.Register("pass.emissive.texture.uv",           Bind(OnPassEmissiveTextureUv,          mat), Parser::needValue);
				parser.Register("pass.fresnel");
				parser.Register("pass.fresnel.color",                 Bind(OnPassFresnelColor,               mat), Parser::needValue);
				parser.Register("pass.fresnel.power");
				parser.Register("pass.fresnel.power.texture",         Bind(OnPassFresnelPowerTexture,        mat));
				parser.Register("pass.fresnel.power.texture.image",   Bind(OnPassFresnelPowerTextureImage,   mat), Parser::needValue);
				parser.Register("pass.fresnel.power.texture.offset",  Bind(OnPassFresnelPowerTextureOffset,  mat), Parser::needValue);
				parser.Register("pass.fresnel.power.texture.scale",   Bind(OnPassFresnelPowerTextureScale,   mat), Parser::needValue);
				parser.Register("pass.fresnel.power.texture.uv",      Bind(OnPassFresnelPowerTextureUv,      mat), Parser::needValue);
				parser.Register("pass.fresnel.power.value",           Bind(OnPassFresnelPowerValue,          mat), Parser::needValue);
				parser.Register("pass.fresnel.texture",               Bind(OnPassFresnelTexture,             mat));
				parser.Register("pass.fresnel.texture.image",         Bind(OnPassFresnelTextureImage,        mat), Parser::needValue);
				parser.Register("pass.fresnel.texture.offset",        Bind(OnPassFresnelTextureOffset,       mat), Parser::needValue);
				parser.Register("pass.fresnel.texture.scale",         Bind(OnPassFresnelTextureScale,        mat), Parser::needValue);
				parser.Register("pass.fresnel.texture.uv",            Bind(OnPassFresnelTextureUv,           mat), Parser::needValue);
				parser.Register("pass.gloss");
				parser.Register("pass.gloss.texture",                 Bind(OnPassGlossTexture,               mat));
				parser.Register("pass.gloss.texture.image",           Bind(OnPassGlossTextureImage,          mat), Parser::needValue);
				parser.Register("pass.gloss.texture.offset",          Bind(OnPassGlossTextureOffset,         mat), Parser::needValue);
				parser.Register("pass.gloss.texture.scale",           Bind(OnPassGlossTextureScale,          mat), Parser::needValue);
				parser.Register("pass.gloss.texture.uv",              Bind(OnPassGlossTextureUv,             mat), Parser::needValue);
				parser.Register("pass.gloss.value",                   Bind(OnPassGlossValue,                 mat), Parser::needValue);
				parser.Register("pass.mask");
				parser.Register("pass.mask.texture",                  Bind(OnPassMaskTexture,                mat));
				parser.Register("pass.mask.texture.image",            Bind(OnPassMaskTextureImage,           mat), Parser::needValue);
				parser.Register("pass.mask.texture.offset",           Bind(OnPassMaskTextureOffset,          mat), Parser::needValue);
				parser.Register("pass.mask.texture.scale",            Bind(OnPassMaskTextureScale,           mat), Parser::needValue);
				parser.Register("pass.mask.texture.uv",               Bind(OnPassMaskTextureUv,              mat), Parser::needValue);
				parser.Register("pass.mask.value",                    Bind(OnPassMaskValue,                  mat), Parser::needValue);
				parser.Register("pass.normal");
				parser.Register("pass.normal.texture",                Bind(OnPassNormalTexture,              mat));
				parser.Register("pass.normal.texture.image",          Bind(OnPassNormalTextureImage,         mat), Parser::needValue);
				parser.Register("pass.normal.texture.offset",         Bind(OnPassNormalTextureOffset,        mat), Parser::needValue);
				parser.Register("pass.normal.texture.scale",          Bind(OnPassNormalTextureScale,         mat), Parser::needValue);
				parser.Register("pass.normal.texture.uv",             Bind(OnPassNormalTextureUv,            mat), Parser::needValue);
				parser.Register("pass.specular");
				parser.Register("pass.specular.color",                Bind(OnPassSpecularColor,              mat), Parser::needValue);
				parser.Register("pass.specular.power");
				parser.Register("pass.specular.power.texture",        Bind(OnPassSpecularPowerTexture,       mat));
				parser.Register("pass.specular.power.texture.image",  Bind(OnPassSpecularPowerTextureImage,  mat), Parser::needValue);
				parser.Register("pass.specular.power.texture.offset", Bind(OnPassSpecularPowerTextureOffset, mat), Parser::needValue);
				parser.Register("pass.specular.power.texture.scale",  Bind(OnPassSpecularPowerTextureScale,  mat), Parser::needValue);
				parser.Register("pass.specular.power.texture.uv",     Bind(OnPassSpecularPowerTextureUv,     mat), Parser::needValue);
				parser.Register("pass.specular.power.value",          Bind(OnPassSpecularPowerValue,         mat), Parser::needValue);
				parser.Register("pass.specular.texture",              Bind(OnPassSpecularTexture,            mat));
				parser.Register("pass.specular.texture.image",        Bind(OnPassSpecularTextureImage,       mat), Parser::needValue);
				parser.Register("pass.specular.texture.offset",       Bind(OnPassSpecularTextureOffset,      mat), Parser::needValue);
				parser.Register("pass.specular.texture.scale",        Bind(OnPassSpecularTextureScale,       mat), Parser::needValue);
				parser.Register("pass.specular.texture.uv",           Bind(OnPassSpecularTextureUv,          mat), Parser::needValue);
				return parser;
			}
		}

		Material *LoadNativeMaterial(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Material> mat(new Material);
			GetParser(*mat).Parse(*stream);
			return mat.release();
		}

		LoadFunction GetNativeMaterialLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeMaterial : LoadFunction();
		}

		REGISTER_LOADER(Material, GetNativeMaterialLoader,
			"mat,material,pagemat,pagematerial", "", "", true)
	}
}
